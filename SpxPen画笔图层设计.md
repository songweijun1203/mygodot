# SpxPenMgr 与画笔图层设计

本文档梳理 `godot_modules/spx` 中画笔（Pen）子系统的类划分、与 Godot 渲染模型的关系，以及为什么这些类里会出现大量 Godot 成员。描述的是本仓库当前代码（Godot 为 `goplus/godot` 的 `spx4.4.1` 分支）。

| 文件 | 内容 |
| --- | --- |
| `godot_modules/spx/spx_pen.h` / `spx_pen.cpp` | `SpxPen`：单支笔的状态机，不渲染任何东西 |
| `godot_modules/spx/spx_pen_mgr.h` / `.cpp` | `SpxPenMgr`：所有者、ABI 门面、坐标翻转、批量命令、flush 时机 |
| `godot_modules/spx/spx_pen_surface.h` / `.cpp` | `SpxPenSurface`（共享图层）、`SpxPenCanvas`（命令 → RenderingServer） |
| `godot_modules/spx/spx_object_mgr.h` / `spx_manager.h` | manager 基类：ID 表、主线程约束、重入安全的遍历 |
| `godot_modules/spx/spx_coordinate.h` | SPX ↔ Godot 的 Y 轴翻转约定 |
| `godot_modules/spx/spx_engine.cpp:183-191` | 帧内 flush 的位置 |
| `godot_modules/spx/spx_layer_sorter.cpp:203-215` | 角色 z_index 从 1 起，给笔迹层留出 0 |
| `godot_modules/spx/gdextension_spx_ext.cpp:304-368` | ABI C 函数薄封装（手写转发到 `penMgr`） |

## 1. 一句话概括

**把 Scratch 的"画笔是一个跨帧持久的共享图层"翻译成 Godot 的"一个 `SubViewport` 渲染目标 + 往它的 canvas item 上累加三角批次"**：逻辑层（笔的状态）留在纯 C++，只有需要被渲染的部分才进场景树。

类里那些 Godot 成员因此不是随意的——它们分成三类，每类有不同的存在理由，见第 4 节。

## 2. 产品语义决定了架构

Scratch 的 pen 有四条语义，和 Godot 的默认用法直接冲突：

| Scratch 语义 | 冲突点 |
| --- | --- |
| 笔迹是**共享图层**，所有角色画到同一层；角色被删除，笔迹还在 | `_draw()` 队列每帧重建，节点一销毁内容就没了 |
| `stamp` 是把外观**写回**那张持久像素图 | `_draw()` 只能表达"这一帧长这样"，无法累积 |
| 圆头笔帽、笔画跨帧连续、只有 `erase all` 才清空 | `canvas_item_add_line` 是直角线段，没有圆帽 |
| "碰到颜色 / 触摸颜色"要**读回**笔迹像素 | GPU 资源读回需要显式同步，且代价高 |

结论：必须有一个"可写回、不清屏、持久存在"的渲染目标。这就是 `SpxPenSurface` 存在的全部理由，也解释了它为什么必须是 Node。

## 3. 四个类的职责

| 类 | 是否 Godot 节点 | 职责 |
| --- | --- | --- |
| `SpxPenMgr` | 否。`SpxObjectMgr<SpxPen>` → `SpxManager` | 所有者 + ABI 门面：查表、坐标翻转、批量命令解码、决定 flush 时机 |
| `SpxPen` | 否，纯 C++ 对象 | 一支笔的状态机：属性、落笔/抬笔、上一点、起点帽、抽稀阈值、印章纹理缓存 |
| `SpxPenSurface` | 是，`Node2D` | 共享图层本体：内含 `SubViewport` + `SpxPenCanvas` + `Sprite2D` |
| `SpxPenCanvas` | 是，`Node2D` | `DrawCommand` 队列 → `RenderingServer` canvas item 的顶点与纹理提交 |

```text
SpxPenMgr (非 Node)
├─ HashMap<GdObj, SpxPen*>  ────▶ SpxPen × N  (非 Node，全部指向同一个 surface)
└─ SpxPenSurface *surface   (Node2D, z=0 绝对层，挂在 spx root 下)
   ├─ SubViewport "pen_render_target"        ← 持久像素在这：透明背景、不清屏
   │   └─ SpxPenCanvas "pen_canvas_drawer"   ← 命令消费端，创建 canvas item RID
   └─ Sprite2D "pen_canvas"                  ← 把上面那张纹理贴回主画面
```

**"共享图层"的实现方式值得单独说**：不是全局单例，而是复用基类的 `root` 字段当共享句柄——

```cpp
// spx_pen_mgr.cpp:97-101
surface = memnew(SpxPenSurface);
root = surface;                       // SpxObjectMgr::root 是 Node2D*
get_spx_root()->add_child(surface);

// spx_pen.cpp:45-47
void SpxPen::on_create(GdInt p_id, Node *p_root) {
    surface = static_cast<SpxPenSurface *>(p_root);   // 基类 _create_object 传进来的 root
```

于是每支笔拿到的都是同一个图层指针，而**笔与笔之间只共享图层、不共享状态**：`last_draw_pos`、`needs_start_cap`、颜色粗细都是每支笔自己的。两支笔交叉绘制时，各自的起笔圆帽互不干扰——这正是 Scratch 多角色同时画画需要的行为。

## 4. 为什么里面有 Godot 成员——三类成员，三种理由

### 4.1 值类型借用：`Vector2 / Color / Transform2D / Rect2 / Size2i / Ref<T>`

这些不是场景对象，是数学类型与资源句柄。直接用引擎类型而不是再造一套，理由很实际：

- **`RenderingServer` 的 API 只吃这些类型**（`canvas_item_add_triangle_array` 要 `Vector<Point2>` / `Vector<Color>` / `Rect2`）；
- `Color::set_hsv`、`Vector2::distance_to`、`Transform2D::affine_inverse` 这些能力自己实现一遍既无意义又容易错。

分工规律是明确的：**`Gd*` 类型只存在于 ABI 边界，一进 C++ 层立刻换成 Godot 类型**。

```cpp
// spx_pen_mgr.cpp:233-235
void SpxPenMgr::move_pen_to(GdObj obj, GdVec2 position) {
    SPX_WITH_PEN_OR_RETURN(obj, pen->move_to(spx_to_godot_vec2(position)))
}
```

翻转只在 manager 边界发生一次。`spx_coordinate.h:36-38` 把它写成了约定：*SPX 用 Y 向上、Godot 用 Y 向下，在 manager API 边界立即转换，使 Godot 节点与物理 API 只接收 Godot 坐标*；`godot_to_spx_vec2` 只是同一个运算的语义别名。

所以 `SpxPen::_append_current_point_if_needed` 里全是原生的 `distance_to` 运算，没有一次类型转换。**这一类成员借的是 Godot 的数学库，不是它的场景模型。**

### 4.2 场景节点成员：`SubViewport *` / `SpxPenCanvas *` / `Sprite2D *`

这三个**必须**是 Node，因为 Godot 的硬规则：不进场景树（或没有 canvas item RID）的东西不会被渲染。它们全部在 `initialize()` 里用 `memnew` + `add_child` 手工搭建，不来自 `.tscn`——图层属于引擎内部机制，不该出现在用户场景里。

关键的几个属性设置（`spx_pen_surface.cpp:234-271`）都是针对 Godot 规则做的选择：

| 设置 | 为什么 |
| --- | --- |
| `set_z_as_relative(false)` + `set_z_index(0)` | 注释写明：Scratch 把笔迹夹在背景与所有角色之间。用**绝对 z**，结果不依赖场景树插入顺序，也不受将来父级 z 变化影响。角色层由 `SpxLayerSorter::_apply_z_index_merged` 从 `z = 1` 起编号，正好把这个 0 留给笔迹 |
| `set_transparent_background(true)` | 图层只有笔迹，其余要透出背景 |
| `set_clear_mode(CLEAR_MODE_ONCE)` + `set_update_mode(UPDATE_ONCE)` | 只在真正有新内容时渲染一次；不每帧重画 |
| `set_disable_3d(true)` / `set_handle_input_locally(false)` | 不当 3D 用、不吃输入——笔迹图层不该参与 GUI 拾取，否则会抢走按钮点击 |
| `Sprite2D` 用 `TEXTURE_FILTER_NEAREST` | 笔迹要像素级清晰，不能被插值糊掉 |
| `Sprite2D` 挂 `BLEND_MODE_PREMULT_ALPHA` 材质 | 见 4.3 |
| `canvas->set_position(Size2 * 0.5)` | `SubViewport` 的原点在左上角，而所有绘制命令用**舞台坐标**（中心为原点）。把 canvas 挪到中心，命令里就不必再手动加偏移（注释在 `:284`） |

### 4.3 RID 与 `Ref` 的生命周期：`DrawItem` 这个结构为什么要存在

```cpp
// spx_pen_surface.h:68-73
// Keep resources alive for exactly as long as their RenderingServer item.
struct DrawItem {
    RID rid;
    Ref<Texture2D> texture;
    Ref<Material> material;
};
```

这是 Godot 混合层最经典的一个坑：**`RID` 不增加 `Ref` 的引用计数**。你把纹理交给 `canvas_item` 去画，引擎不会替你保住那个资源对象；如果 `Ref` 提前析构，纹理被卸载，RID 画出来的就是垃圾或空。

作者的处理是把"RID 的存活期"和"资源的引用"绑进同一个 `Vector` 元素，同生共死：`_create_draw_item()` push 一条，`_clear_draw_items()` 成对 `RenderingServer::free(rid)` 再 `clear()`，析构函数里也调 `_clear_draw_items()`（`:227-229`）。**规则被编码进了数据结构本身**，而不是写在注释里靠人记。

另外三处也是同源的 Godot 规则：

- **预乘 alpha**（`:263-268`）：透明渲染目标里存的是预乘 RGB，普通混合会再乘一次 alpha，边缘出现黑边。所以显示用的 `Sprite2D` 必须换成 `PREMULT_ALPHA` 混合；`capture` 回填的快照也标 `image_premultiplied = true`，让像素感应按同样的约定解读。
- **material 的浅拷贝冻结**（`spx_pen_surface.cpp:213-218`）：印章取的是精灵当前材质，用 `material->duplicate(false)` —— 共享纹理和 shader 代码，但**把材质参数在这一刻冻住**。否则队列里的旧笔画会随着角色身上的特效变化而变样，不符合"写回"的语义。
- **绘制空间用逆变换换算**（`:312`）：`get_global_transform().affine_inverse() * p_sprite->get_global_transform()`，把角色的全局变换换算到笔迹图层的局部空间。图层自身可能被移动或缩放，直接抄角色的变换会错位。

## 5. 帧时序：flush 为什么挂在那个位置

```cpp
// spx_engine.cpp:183-191
_notify_managers(&SpxManager::on_update, delta);   // 各 manager 自己更新（含笔跟随鼠标）
if (callbacks.func_on_engine_update) {
    callbacks.func_on_engine_update(delta);        // 脚本 / GDExtension 的一帧逻辑，笔指令在这里下达
}
if (pen) {
    pen->flush_all();                              // 一帧的指令攒成一批，提交给 RenderingServer
}
```

顺序不能换：脚本（`gdexample` 的 `Game::OnEngineUpdate` 就落在第二个环节）在这一帧内发出的所有 pen 指令必须进**同一个批次**。如果每调用一次就 flush，同一条笔画会被拆成多次提交，带来两个问题——多次 `canvas_item` 批次、以及第 6 节说的"重复清屏覆盖"。

`flush()` 里的 `append` 语义是这段代码最容易看漏的地方：

```cpp
// spx_pen_surface.cpp:315-334
const bool pending = _is_render_pending();
if (clear_requested || !pending) {
    render_target->set_clear_mode(clear_requested ? CLEAR_MODE_ONCE : CLEAR_MODE_NEVER);
}
canvas->submit(pending);                 // pending → 追加，不销毁已有 draw item
render_target->set_update_mode(UPDATE_ONCE);
collision_image.unref();                 // 有新内容 → 像素缓存作废
```

注释（`:320-321`）说清了依据：**`UPDATE_ONCE` 是被渲染器消费的，不是被 `set_update_mode` 重置的**。`_is_render_pending()` 直接去问 `RenderingServer::viewport_get_update_mode() == VIEWPORT_UPDATE_ONCE`（`:332-334`），而不是自己记一个 bool。所以一帧内 flush 两次（比如中途发生像素感应）时，第二批必须**追加**而不是清空重画，否则第一批笔迹连同它的 clear 请求一起丢掉。

同样的道理决定了 `clear()` 是**延迟生效**的：

```cpp
void SpxPenSurface::clear() { canvas->clear_commands(); clear_requested = true; }
```

它只立标记，真正的 `CLEAR_MODE_ONCE` 要等到 flush 那一刻才写进渲染目标。这样"清屏"和"提交新内容"是原子的，中间插入的 `capture()` 不会看到一个既没旧笔迹也没新笔迹的空帧。

## 6. 笔迹持久化：不重放历史，只累加新内容

这是与 Godot 常规 `_draw()` 用法**相反**的选择，值得点明：

- Godot 惯用法是"每帧重建整个绘制队列"，即数据源在别处，`_draw()` 只是投影；
- 笔迹图层的数据源**就是那张纹理本身**，所以 `submit(false)` 只在真正需要重画时才销毁历史 draw item，其余时间都是 `canvas_item_create` 追加子项、渲染器累加进不清屏的 `SubViewport`。

历史不保存。`erase all` 之所以简单，就是因为它等价于"销毁所有 draw item + 让下一次渲染清屏"。

代价也要知道：笔迹可写范围固定为渲染目标大小（`on_awake` 按 `viewport->get_visible_rect().size` 初始化，默认回退 480×360）。**画到目标之外会被直接裁掉**；`set_canvas_size` 改变尺寸时必须 `clear()`（`:273-287`），因为像素图不能缩放，只能重建。

## 7. 一条线怎么变成 GPU 三角形

`SpxPenCanvas::_draw_line_batch`（`spx_pen_surface.cpp:52-141`）把**连续的一段 LINE 命令合并成一次 `canvas_item_add_triangle_array`**：

- 每条线段 = 一个 quad（4 顶点 6 索引，由线段法向 × 半径挤出）+ 末端一个扇形圆盘（`CAP_SEGMENTS = 12`）；
- 零长度命令直接退化成圆盘 —— 这就是"画一个点"的实现，`on_down` 和"有颜色但没有上一点"都走这条分支；
- 先按命令数精确算出所需顶点/索引数，`resize` 一次到位，再用 `ptrw()` 裸写填充，末尾 `DEV_ASSERT` 校验数量一致（`:136-137`）。**没有中间容器，没有逐条 append 扩容**。

为什么要自己铺顶点而不用 `canvas_item_add_line`：后者是直角线段、没有圆帽，而且 N 条线是 N 次调用；Scratch 的笔是圆头的，一条连续笔画可能有几百段。批处理顺带解决了另一个问题——同一批用同一个 draw item，一次提交。

`draw_start_cap` 这个字段的设计意图在注释里（`:128-129`）：*Scratch 用圆头笔帽；同一条笔画复用前一段的端点圆帽，即使这些段被分到不同帧 flush*。`SpxPen` 侧对应的三个状态就是为它服务的：

| 状态 | 作用 |
| --- | --- |
| `needs_start_cap` | 本段是否是**新笔画**的第一段（是则两端都画帽，否则只画终点帽，避免重复叠色） |
| `has_last_draw_pos` | 是否已有点可以连线（没有就先画一个点） |
| `min_draw_distance = 1.0f` | 抽稀阈值：移动不足 1px 不画。**不加这层，原地抖动会在同一位置反复 alpha 混合，线条颜色越描越深** |
| `_get_draw_position` 的 `+0.5` | 宽度为 1 或 3 时，线中心正好压在像素边界上会糊成两半，偏移半像素对齐像素网格 |

另一处细节：**任何属性变化都会 `_start_new_line()`**（`set_color_to` / `change_by` / `set_to` / `change_size_by` / `set_size_to` 全部调用它）。于是一条中途变粗或变色的笔画，会在接缝处补一个圆帽、重新起一段，避免出现宽度突变造成的缺口。

`_get_current_color()` 把 Scratch 的三属性合成最终颜色：基色的 H 保留，S/V 分别乘 `saturation` / `brightness`，alpha 取 `transparency`（走 `Color::set_hsv` 的第四个参数）。

## 8. 像素感应：capture 的同步与缓存策略

`SpxPenSurface::capture`（`:336-367`）是给"碰到颜色 / 触摸颜色"用的，它把几个 Godot 层的昂贵操作按代价从小到大层层短路：

```text
1. 世界包围盒重叠判断（SpxPixelQuery::overlap）   ← 不重叠直接 return false，零同步开销
2. flush()                                        ← 确保本帧指令已进渲染队列
3. 若渲染尚未提交 → 直接 server->draw(false) + server->sync()
4. render_target->get_texture()->get_image()      ← 读回 CPU 像素，缓存进 collision_image
5. 任何新的 flush() → collision_image.unref()      ← 用失效标记维持缓存一致性
```

第 3 步的注释（`:350-353`）是关键：**不能等全局的 deferred 调用队列**——脚本在同一帧里刚画的笔迹，感应必须立刻看得见。所以它手动 `canvas->force_update_transform()`、`CanvasItemMaterial::flush_changes()`、再 `RenderingServer::draw(false)` + `sync()`，把这一层单独推到 GPU 完成。

`collision_image` + `flush()` 里那句 `unref()` 构成一个很朴素的缓存协议：只要有人画了新东西，缓存立刻作废，下一次 capture 重新读回。**笔迹像素读回是全图层级别的**（读整张纹理），所以只有查询区域真 overlaps 时才值得做——第 1 步存在的意义就在这里。

调用方是 `spx_sprite_mgr.cpp:1034`：`pen->capture(self_query.bounds, pen_layer.pixel_query)`，把笔迹图层作为一个可感应的"层"参与像素碰撞，z 与层序来自第 4.2 节的那套约定。

## 9. 批量 ABI：`batch_update_commands`

除了逐个调用的 ABI，还有一条**批量指令通道**，一帧一次调用代替几十次单独调用。它是给跨语言的脚本运行时（Go 侧）设计的，但你项目里也已经 resolve 好了（`gdexample/src/spx_api.cpp:88-90`），只是 `gdexample/src` 目前还没有调用它：

```text
[count] + count × [op, idLowBits, idHighBits, a, b, c, d, reserved]     // 每条 8 个 float
op: 1=MOVE  2=DOWN  3=UP  4=COLOR(rgba)  5=SET_SIZE
```

三个设计选择值得注意：

1. **整个缓冲区只有 `float` 一种元素类型**。跨语言侧因此不必构造异构数组；`GdObj`（64 位，可能是指针）用两个 float 的**位模式**承载：
   ```cpp
   uint32_t read_u32_lane(float p_value) { uint32_t bits; std::memcpy(&bits, &p_value, 4); return bits; }
   ```
   用 `memcpy` 而不是 `reinterpret_cast` 做 type punning——合法、不违反严格别名、可移植。`gd_obj_from_i64<T>` 再用 `if constexpr` 处理 `GdObj` 是指针类型的情况。
2. **先整体校验，再执行**（`:144-164`）：长度、count 是否为整、每条 op 是否合法，全部检查通过才开始改动笔状态。所以不会出现"执行了一半发现格式错"的半生效状态。
3. **缺失对象只记账不中断**（`:166-197`）：某条记录指向已销毁的笔时跳过该条、继续处理其余，最后统一一次 `print_error`。批量通道里逐个报错会把日志刷爆。

其余 ABI 方法都走同一个形状。`SPX_BIND` 本身是个**空的标记宏**（`spx_abi.h:41-42`，注释：*Exported cross-language method*），不做任何代码生成——真正的 C 边界是 `gdextension_spx_ext.cpp:304-368` 里手写的转发函数，每个 `spx_pen_*` 导出函数就是取 `penMgr` 再调一行。看到 `SPX_BIND` 就该知道"这个方法在 ext.cpp 里有一个对应的 C shim"，两边靠人保持同步。

## 10. 框架规则：基类给了什么

`SpxPenMgr` 的写法完全由两个基类的规矩决定，这也是整个 SPX 模块的架构约定：

```cpp
// spx_manager.h:41  注释
// Managers opt into lifecycle hooks; scene nodes belong to their actual users.
class SpxManager { virtual void on_awake() {} /* start/update/fixed_update/destroy/reset/exit/pause/resume */ };
```

这条解释了为什么 `SpxPenMgr` **不是** Node：管理器不靠"待在树里顺便接收 `_process`"，而由 `SpxEngine::_notify_managers` 显式驱动；需要被渲染或被引擎当作场景对象处理的（图层、canvas、sprite）才是 Node。你之前看到的 `SpxUiMgr` 同样遵循这条：mgr 非 Node，Control 是 Node。

```cpp
// spx_object_mgr.h:39-41  注释
// Owns non-Node objects and their shared scene root on the engine main thread.
// Returned pointers must not escape the current main-thread task; callers from
// other threads must enqueue work before entering the manager.
template <typename T> class SpxObjectMgr : public SpxManager { HashMap<GdObj, T *> id_objects; Node2D *root; ... };
```

模板基类提供的四件事，`SpxPenMgr` 全都用上了：

| 机制 | 代码 | 为什么这样 |
| --- | --- | --- |
| 主线程约束 | 每个可能改表或碰 RID 的入口先 `_require_main_thread(__func__)`，非主线程 `ERR_PRINT` 并返回 | `HashMap` / RID 都不是线程安全的；宁可拒绝也不产生数据竞争 |
| 重入安全的遍历 | `_update_all` **先收集 id、再逐个重新查表**调用（`:157-164` 有注释） | 某支笔的 `on_update` 里销毁了后面的笔，不会留悬垂指针 |
| 重入安全的销毁 | `_destroy_objects_and_root` 先 `id_objects.clear()` 再逐个 `on_destroy()` + `memdelete()` | 任何回调都看不到半删除的表 |
| 查表 + 判空 + 调用三件套 | `with_object` / `with_object_ret` + `SPX_WITH_PEN_OR_RETURN` 宏 | 17 个 `SPX_BIND` 方法里，凡"操作某一支笔"的 11 个都被压成同一形状的一行 |

三处属于 `SpxPenMgr` 自己补的、容易被忽略的细节：

- **`on_destroy` 必须先 `surface = nullptr;` 再调 `_destroy_objects_and_root()`**（`spx_pen_mgr.cpp:114-117`）。基类只清 `root`，而 `surface` 是同一个指针的**别名**，不手动清就留下悬垂成员。
- **`destroy_all_pens` 的顺序**（`:200-211`）：先 `surface->clear()`（图层要干净），再对每支笔 `on_erase_all()`（`has_last_draw_pos = is_pen_down` —— 让落笔状态的笔在清屏后的第一笔重新带起点帽，而不是从已经不存在的旧点连线）。
- **跨 manager 取对象走统一的 checked lookup**（`:241-248`）：`pen_stamp_sprite` 用 `spx_checked_lookup<SpxSprite>(sprite_id, __func__, spriteMgr, ...)` 而不是直接摸 `spriteMgr` 的表，失败路径与别的 API 一致。

## 11. 我认为值得你确认的四点

这几条是读代码得到的，不是猜测，但我没有运行验证，标注清楚：

1. **`on_create` 里 `static_cast<SpxPenSurface*>(p_root)` 没有类型校验**。它依赖"`SpxPenMgr::root` 一定是 `SpxPenSurface`"这个约定（现在成立）。一旦外面多包一层节点或换成别的 root，就是静默 UB。建议改成 `DEV_ASSERT(Object::cast_to<SpxPenSurface>(p_root))`，或者让 `SpxPen` 提供一个 `set_surface(SpxPenSurface*)` 供 mgr 显式调用，别借 `Node*` 传。
2. **`move_by_mouse` 的坐标来源和别处不同**。`spx_pen.cpp:118-122` 用的是 `Input::get_singleton()->get_mouse_position()`（窗口像素、左上原点、无相机、无 stretch 换算），而脚本侧的鼠标坐标在 `spx_input_mgr.cpp:71` 用的是 `cameraMgr->get_global_mouse_position()`。两个空间不同源：**相机移动、`zoom != 1`、或窗口 stretch 与舞台尺寸不一致时，"鼠标按下拖动画画"会跟鼠标错位**。如果这条路径要真用，应该复用 camera 的全局鼠标坐标。
3. **`transparency` 的名字与语义方向相反**。结构体默认 `transparency = 0.0f`（`spx_pen.h:58`），`on_create` 重置成 `1.0f`，最终作为 `Color::set_hsv(h, s, v, alpha)` 的第四个参数——**值越大越不透明**。而 Scratch 的 pen transparency 是"越大越透明"。需要确认脚本/Go 侧传进来时有没有做取反；如果没做，`change_pen_by(2, +10)` 的效果方向和 Scratch 相反。
4. **笔的默认属性写在两处**：结构体成员初始化（`color` 黑、`size = 2`、`saturation/brightness = 1`、`transparency = 0`）和 `on_create`（只重置了 `transparency`、状态位和 `stamp_texture_path`）。目前是 `memnew` 全新对象所以没出问题，但以后若改成对象池或复用，`on_create` 的重置清单不完整就会串状态；即使不改池，改一处默认值也容易漏掉另一处。

## 12. 一图总结数据流

```text
脚本 / Go 运行时
   │  spx_pen_down / spx_move_pen_to / batch_update_commands(float[])
   ▼
gdextension_spx_ext.cpp  ── 手写 C shim ──▶  SpxPenMgr (SPX_BIND)
                                                │ spx_to_godot_vec2：Y 翻转，一次
                                                ▼
                                     SpxPen（状态机：属性 / 落抬笔 / 上一点 / 帽）
                                                │ draw_line / draw_stamp（只入队）
                                                ▼
                                     SpxPenSurface (Node2D, z=0)
                                                │ canvas->add_*  → pending_commands
        SpxEngine::on_update 末尾 flush_all() ──┘
                                                ▼
                                     SpxPenCanvas::submit(append)
                                        ├─ LINE 连续段 → 顶点/索引批 → canvas_item_add_triangle_array
                                        └─ STAMP → canvas_item + texture draw_rect_region
                                                ▼
                                     SubViewport（不清屏 → 笔迹持久）
                                                ▼
                                     Sprite2D（PREMULT_ALPHA + NEAREST）→ 主画面
```
