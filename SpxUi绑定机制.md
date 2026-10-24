# SpxUi 与 UI 绑定机制

本文档梳理 `godot_modules/spx` 中 UI 侧的类划分与职责设计，重点是 `SpxUi` / `SpxUiBinding` / `SpxUiBindingListener` 三者的关系，以及 `_notification` 能被调起来的引擎机制。描述的是本仓库当前代码（Godot 为 `goplus/godot` 的 `spx4.4.1` 分支），不是通用 Godot 结论。

| 文件 | 内容 |
| --- | --- |
| `godot_modules/spx/spx_ui.h` / `spx_ui.cpp` | `SpxUi`：SPX 侧的 UI 记录，ABI 动词到 Control API 的翻译 |
| `godot_modules/spx/spx_ui_binding.h` / `.cpp` | `SpxUiBinding`（探针 Node）、`SpxUiBindingListener`（通知接口） |
| `godot_modules/spx/spx_ui_mgr.h` / `.cpp` | `SpxUiMgr`：所有者、CanvasLayer、事件路由、ABI 导出 |
| `godot_modules/spx/tests/test_spx_ui_binding.h` | 绑定行为的单元测试，也是设计意图的直接证据 |
| `godot/core/object/object.h` / `.cpp` | 通知链宏、`Object::notification`、`_predelete`、`connect` / `emit_signalp` |
| `godot/core/templates/hash_map.h` | 插入序链表，决定信号处理顺序 |
| `godot/core/object/callable_method_pointer.h` | `callable_mp` → 成员函数指针打包成 Callable |
| `godot/scene/gui/base_button.cpp` | `_gui_input` → `_pressed` → `emit_signal("pressed")` |
| `godot/scene/main/node.cpp` | `NOTIFICATION_PREDELETE` 的宿主级处理（摘父、删子） |

## 1. 角色分工

```text
SpxUiMgr ──拥有──▶ SpxUi   （裸 C++ 对象，不在场景树里）
   │                 │  control_id / binding_id   —— 都是 ObjectID，不是指针
   │                 ▼
   │           Godot Control ◀──父节点── SpxUiBinding （Node，在场景树里）
   └── private 继承 ──▶ SpxUiBindingListener ◀──回调──┘
```

| 类 | 一句话职责 |
| --- | --- |
| `SpxUi` | SPX 侧的一条 **UI 账本记录**：把 `gid` ↔ `Control` 关联起来，并把 ABI 调用翻译成 Control 的 setter/getter |
| `SpxUiBinding` | 挂在宿主 Control 底下的 **寄生探针**：替 SPX 监听 `pressed` 信号与宿主的生死 |
| `SpxUiBindingListener` | 探针与账本所有者之间的 **单向通知接口**，只有两个方法 |
| `SpxUiMgr` | 两者的所有者 + 事件路由器；持有 `RBMap<GdObj, SpxUi*>` 和一个统一 `CanvasLayer` |

## 2. 核心矛盾：SPX 操作 Control，但不拥有 Control

- Control 可能被任何人释放：`queue_free`、父节点析构、场景切换、引擎 reset。
- `SpxUi` 若存 `Control*` → 悬垂指针；若存 `ObjectID` → 能安全判活（`ObjectDB::get_instance` 死则返回空），但**没人通知它宿主死了**，于是账本里的记录会永久滞留，`gid` 也不会通知脚本。
- 反向也不行：不能给 `BaseButton` / `Control` 加 SPX 字段和回调。`spx_ui_binding.h:48-50` 的注释讲明了这点——把集成留在模块内，upstream 的类保持原样，fork 才能持续跟上游 rebase。

解法是标准的 Godot 思路：**组合优于继承，借引擎自身的机制当通知通道**。

- 释放父节点必然释放子节点 → `NOTIFICATION_PREDELETE` 是"宿主死了"的可靠信号；
- 点击本来就是标准 `pressed` 信号 → `connect` 即可；
- 于是需要一个"能进树、能收通知、能被自动释放、又不参与绘制和布局"的最小载体——`Node`。不是 `Control`（会多一个矩形，影响布局与输入），也不是 `Resource`（不进树，收不到 PREDELETE）。

```cpp
// spx_ui.cpp:62-64
SpxUiBinding *binding = memnew(SpxUiBinding);
binding->set_name("_spx_ui_binding");
p_control->add_child(binding, false, Node::INTERNAL_MODE_BACK);
```

两处细节：

- **`_` 前缀 + `INTERNAL_MODE_BACK`**：内部子节点在 `get_children()` 的默认调用里不可见、编辑器里不显示，用户遍历自己的场景不会看到这个多出来的孩子，`get_child_count()` 的假设也不被破坏。
- **`INTERNAL_MODE_BACK` 还带来一个顺序保证**：back-internal 子节点排在 `children_cache` 末尾（`node.cpp:468-485`），而 `Node::_notification` 处理 PREDELETE 时是**从末尾开始删子节点**（`node.cpp:255-258`）。所以宿主被释放时，探针先死、其它可见子节点后死——`SpxUi` 在其它孩子销毁之前就从账本里注销了。这条作者没写注释，是从两处代码推断的。

## 3. `SpxUi` 成员含义

| 成员 | 含义与设计考虑 |
| --- | --- |
| `ObjectID control_id` | 宿主。用 ID 而非指针：宿主死了 `get_control_item()` 返回空，因此**所有访问都是现取现用**（`spx_ui.cpp:53-55`），不缓存裸指针 |
| `ObjectID binding_id` | 本次绑定的"代次凭证"，配合 `owns_binding()` 识别过期回调（见第 5 节） |
| `GdObj gid` | 跨语言的唯一身份，由 `get_unique_id()` 分配。ABI 只见整数，`SpxUi*` 永不外泄 |
| `GdInt type` | `ESpxUiType`。创建时用 `dynamic_cast` 链推导（`spx_ui_mgr.cpp:74-107`），**叶子类型在前、`SpxControl` 垫底**，否则 Label 会被判成 Control |

方法分三组：

1. **接线组** `set_control_item / detach_control_binding / owns_binding / get_control_item / ~SpxUi` —— 唯一涉及 binding 的一组；
2. **类型访问组** `get_label / get_button / ...` —— 走 `GET_SPX_CONTROL_TYPE` 宏（`spx_ui.cpp:41-47`），先校验 `type` 再强转，不匹配就 `print_error` 返回空，把 C++ 层的类型误用降级成一次可诊断的运行时错误；
3. **ABI 转发组** `set_text / set_rect / set_position / ...` —— 只做 `Gd*` ↔ Godot 类型转换。

`type` 为什么必要：Control 家族没有统一的 "texture" 概念。`set_texture`（`spx_ui.cpp:229` 起）按 `type` 分派——Button/Toggle → `set_button_icon`，Image → `set_texture`。同理 `set_color` 选 `self_modulate`、`set_font_size` 选 `add_theme_font_size_override("default")`：都是 Label / Button / LineEdit 三者通吃的最小公共能力。`get_canvas_scale()` 取 `get_global_transform_with_canvas().get_scale()`，即累计到画布的缩放，是 `gdexample` 里 Monitor 文字自适应屏幕的数据来源。

## 4. `SpxUiBinding` 成员与两个实现细节

| 成员 | 含义 |
| --- | --- |
| `control_id` | 反向指回宿主，同样用 `ObjectID`，`get_control()` 现查（`spx_ui_binding.cpp:44-46`） |
| `GdObj gid` | 从 `SpxUi` **复制**而来。目的是让 binding 报事件时无需回头找 `SpxUi`，依赖方向保持单向 |
| `SpxUiBindingListener *listener` | 全模块唯一的裸指针。指向 `SpxUiMgr` 单例（活得比所有 binding 久），`detach()` 与析构都会清空 |
| `attach / detach` | `ERR_ALREADY_IN_USE` 拒绝重复绑定；`attach` 失败时**回滚三个成员**（`:107-111`） |

**细节一：`_connect_pressed_first` 为什么要"插到最前面"**（`spx_ui_binding.cpp:48-96`）

旧实现是 SPX 直接改 `BaseButton`，在 `emit_signal("pressed")` **之前**调用 ABI 回调。改成标准信号后，新连接默认追加在已有连接之后，脚本的 `pressed` 处理就会跑到 SPX 回调前面——行为变了。于是它先把已有连接全部摘下（并记录 `reference_count`，因为 Godot 4 允许同一 callable 重复连接），连上自己的，再把别人的原样接回（连 `flags` 一起恢复）。注释里 "including for connections restored from PackedScene" 说的是从 `.tscn` 带出来的连接同样保序。测试里 `events[0]==1`、`events[1]==2` 断言的正是这条顺序。

**细节二：发回调前先把状态清零**（`spx_ui_binding.cpp:139-151`）

```cpp
if (p_what != NOTIFICATION_PREDELETE || listener == nullptr) return;
SpxUiBindingListener *callback = listener;
const GdObj callback_gid = gid;
const ObjectID binding_id = get_instance_id();
listener = nullptr; gid = 0; control_id = ObjectID();   // 先自断
callback->on_spx_ui_destroyed(callback_gid, binding_id); // 后通知
```

因为 `on_spx_ui_destroyed` 会回头操作 `SpxUiMgr`（`erase` map、`memdelete(SpxUi*)`，而 `~SpxUi` 又会 `detach_control_binding()` 反过来找这个 binding）。若先回调后清零，这条重入路径会二次触发或在半销毁状态上打转。`_on_pressed` 里先把 `listener` / `gid` 拷进局部变量再调用，是同一个考虑。

## 5. `SpxUiBindingListener` 接口设计

```cpp
virtual void on_spx_ui_clicked(GdObj p_gid, ObjectID p_binding_id) = 0;
virtual void on_spx_ui_destroyed(GdObj p_gid, ObjectID p_binding_id) = 0;
```

- **只声明探针真有能力观察的两件事**。ABI 里其实还有 `func_on_ui_pressed / released / hovered / toggle / text_changed` 等槽位，但 `SpxUiBinding` 没接——接口没提前膨胀成上帝监听器。
- **为什么多带一个 `ObjectID`**：防过期回调（ABA）。`set_control_item` 开头会 `detach_control_binding()` 再建新 binding；若旧宿主随后才被释放，旧 binding 同样会报 destroyed。`SpxUiMgr::on_spx_ui_destroyed` 用 `node->owns_binding(p_binding_id)`（`spx_ui_mgr.cpp:123`）挡掉不属于当前代次的通知，否则会误删一个刚刚重绑成功、活得好好的 `SpxUi`。
- **依赖方向单向**：`spx_ui_binding.h` 不 include `spx_ui.h`；`spx_ui.h:56` 只前向声明；`spx_ui_mgr.h:37-40` 同时 include 两者并**私有继承**接口。私有继承的语义是"我实现一个内部协议"，不是"我对外的身份是监听器"——否则任何拿到 `SpxUiMgr*` 的人都能伪调 `on_spx_ui_clicked`。把 `this` 传给 `set_control_item(control, this)` 是显式授权。
- **副作用是可测**：`tests/test_spx_ui_binding.h:76-79` 用裸 `Button` + 假 `BindingListener` 就能单测绑定行为，完全不需要初始化 SPX。
- **listener 可以为空**：`set_control_item(control, nullptr)` 合法（测试第 3 个用例）。探针的两个职责——点击转发、销毁通知——可以只取其一；`_notification` 里的 `listener == nullptr` 判断就是为此。

## 6. 一次完整生命周期

```text
create_node("res://engine/ui/UiMonitor.tscn")
 ├─ create_control()  → PackedScene::instantiate() + dynamic_cast<Control*>，失败则 memnew
 └─ on_create_node(control, type, is_attach=true)        spx_ui_mgr.cpp:195-217
      ├─ canvas_layer->add_child(control)     ← SPX 的 UI 全挂在自己的 CanvasLayer 上
      ├─ node->set_type(type); node->set_gid(get_unique_id())   ← 顺序有要求，见第 8 节
      ├─ node->set_control_item(control, this) → new SpxUiBinding → attach
      └─ id_objects[gid] = node; func_on_ui_ready(gid)

点击：BaseButton "pressed" → SpxUiBinding::_on_pressed → SpxUiMgr::on_spx_ui_clicked
        → owns_binding 校验 → SPX_CALLBACK->func_on_ui_clicked(gid)
        → ABI → gdexample/src/GDCallBackHandler.cpp:203

销毁（三条路径收敛到同一个通道）：
 ① destroy_node(obj)       → queue_free ─┐
 ② GD 侧 / 父节点释放 Control ────────────┼→ binding NOTIFICATION_PREDELETE
 ③ on_reset / on_destroy   → detach ─────┘   → on_spx_ui_destroyed
                                              → erase + func_on_ui_destroyed + memdelete(SpxUi)
```

要点：

- 三条路径最后都经 PREDELETE，**注销只有一个入口**，不会出现两处各自 `erase` 导致的二次释放；`on_spx_ui_destroyed` 还用 `id_objects.erase()` 的返回值决定是否通知脚本（`spx_ui_mgr.cpp:127`）。
- `destroy_node` 只 `queue_free` 就返回 true，真正的注销发生在帧尾。写 `gdexample` 时不要假设调用完 `destroy_node` 后 `gid` 立刻失效。
- `_clear_nodes(emit_destroyed, queue_controls)` 用两个布尔区分场景：reset 要通知脚本并释放节点，引擎关闭两者都不做（`spx_ui_mgr.cpp:166-175`）。
- `~SpxUi` 也调 `detach_control_binding()`（`spx_ui.cpp:49-51`），防止 `SpxUi` 先死、binding 还持有野 listener。
- `bind_node(obj, rel_path)`（`spx_ui_mgr.cpp:283-314`）用 `is_attach=false` 调 `on_create_node`，**不改父**——加载的 `.tscn` 里子节点本就挂在宿主下面。这就是 `UiMgr::BindNode` → `Monitor::BindVariantNodes()` 给子节点拿到独立 `gid` 的机制：一个 Control 子树可以有多个 `SpxUi` + 多个 binding，父子各一份，销毁时各自报各自的 `gid`。

## 7. `_notification` 不是虚函数，为什么会被调起来

关键：**虚的不是 `_notification`，而是 `_notificationv`。** `GDCLASS` 宏替每个类生成了这一个虚槽位的覆写，并静态地调用本类的 `_notification`。

引擎侧的声明（`godot/core/object/object.h`）：

```cpp
virtual void _notificationv(int p_notification, bool p_reversed) {}   // :707  虚槽位
void _notification(int p_notification) {}                             // :717  非虚，空实现
void notification(int p_notification, bool p_reversed = false);        // :885  公开入口，非虚
```

`GDCLASS(SpxUiBinding, Node)` 展开出的 `CLASS_DESC` 宏体（`object.h:551-564`）：

```cpp
_FORCE_INLINE_ void (Object::*_get_notification() const)(int) {
    return (void (Object::*)(int)) & m_class::_notification;      // 本类作用域解析
}
virtual void _notificationv(int p_notification, bool p_reversed) override {
    if (!p_reversed) { m_inherits::_notificationv(p_notification, p_reversed); }  // 正向：先父
    if (m_class::_get_notification() != m_inherits::_get_notification()) {
        _notification(p_notification);                           // 静态调用，非虚
    }
    if (p_reversed) { m_inherits::_notificationv(p_notification, p_reversed); }   // 反向：后父
}
```

三点值得注意：

1. **通知链是编译期由宏逐级拼出来的静态调用链**，不靠"子类 override 基类虚函数"。`m_inherits::_notificationv(...)` 是带类名的限定调用，沿 `GDCLASS` 的第二参数逐级向上：`SpxUiBinding → Node → Object`（`node.h:49` 是 `GDCLASS(Node, Object)`），或按控件走 `Button → BaseButton → Control → Node → Object`。派生类不需要（也无法）显式调用父类的 `_notification`。
2. **`_get_notification()` 是一个能力探测**：比较两个成员函数指针，判断本类到底自己写了 `_notification` 还是继承了上一级的空壳。没写就跳过调用，省一次空跳转。`node.h:344` 的 `void _notification(int p_notification);` 同样没写 `virtual`，靠的就是这套机制。
3. **`p_reversed` 决定顺序**：正向通知（`NOTIFICATION_READY` 等）基类在前，反向通知（`NOTIFICATION_PREDELETE`）派生在前。这与析构语义一致。

销毁时的完整调用路径（每一步都可对源码验证）：

```text
memdelete(button)                                       core/os/memory.h:134-144
 └─ predelete_handler(p_class)     非 Object 类命中 void* 版直接返回 true；
                                   Object 派生类命中 object.h:1007 那一版
    └─ Object::_predelete()                             core/object/object.cpp:228-236
       └─ notification(NOTIFICATION_PREDELETE, true)  ← p_reversed = true
          └─ Object::notification()                    core/object/object.cpp:905-931
             ├─ script_instance->notification(...)     脚本在前
             ├─ _extension->notification2(...)          GDExtension 类走这里
             └─ _notificationv(...)                    虚分发 → 最派生类宏生成的版本
                └─ Button 的宏版：Button::_notification → BaseButton → Control
                   → Node::_notification(PREDELETE)    scene/main/node.cpp:234-259
                      ├─ data.parent->remove_child(this)     摘出父节点
                      └─ while (children) memdelete(children.last())   从末尾开始删子
                         └─ memdelete(_spx_ui_binding)  ← 回到本框顶部，递归
                            └─ SpxUiBinding::_notification(NOTIFICATION_PREDELETE)
                               └─ listener->on_spx_ui_destroyed(gid, binding_id)
```

所以保证来自三件事叠加：**① `GDCLASS` 宏生成了真正带 `virtual`/`override` 的转发函数；② 成员函数指针比较让"本类是否实现 `_notification`"在编译期被探测；③ 对象析构统一走 `memdelete → predelete_handler → _predelete → notification(PREDELETE, reversed)`，而 Node 在这一步会递归删掉自己的子节点**——探针作为宿主的子节点，必然被这条链触达。

一句话：`_notification` 靠"名字所在的作用域"被选中，不靠 vtable；vtable 里的是宏生成的 `_notificationv`。也正因如此，**给一个类接上通知能力的开关就是 `GDCLASS` 宏**——去掉它，`_notification` 会退化成一个人人可写但永远没人调的普通成员函数。

顺带一条对照：GDExtension 侧的类（`godot-cpp` 生成的 `godot::Node` 子类，比如 `gdexample` 的 `GdUiNode`）不在这个宏体系里，它们的 `_notification` 走 `Object::notification` 中 `_extension->notification2` 那条分支。（当前 `gdexample/src` 里没有类重写 `_notification`，这条分支还是空的。）

## 8. `_on_pressed` 是怎么被调起来的

`_on_pressed` 也是私有、非虚，但它被调起来的机制和第 7 节完全不同：**它的函数地址在连接那一刻就被固化进一个 Callable 里**。

```cpp
// spx_ui_binding.cpp:55,76
const Callable binding_callable = callable_mp(this, &SpxUiBinding::_on_pressed);
button->connect(pressed_signal, binding_callable);
```

`callable_mp` 宏（`core/object/callable_method_pointer.h:219`）展开成 `create_custom_callable_function_pointer(T *, const char *, void (T::*)(P...))`，内部 `memnew` 一个 `CallableCustomMethodPointer`，它打包了一块 POD：

```cpp
struct Data {
    T *instance;
    uint64_t object_id;
    R (T::*method)(P...);      // ← 成员函数指针，connect 时就确定
} data;
```

于是运行时既不需要虚表，也不需要宏生成的调用链——拿着 `(instance->*method)(...)` 直接调。私有成员能取地址，是因为 `callable_mp` 写在 `_connect_pressed_first` 这个本类成员函数里。

完整链路：

```text
鼠标事件 → Viewport::_gui_input_event → Control::gui_input
└─ BaseButton::_gui_input()                        scene/gui/base_button.cpp:150-186
   └─ BaseButton::_pressed()                                                      :139
      ├─ GDVIRTUAL_CALL(_pressed)      ← 第三套机制，见下表
      └─ emit_signal(SceneStringName(pressed))                                    :142
         └─ Object::emit_signalp()     core/object/object.cpp:1166
            ├─ signal_map["pressed"] → SignalData::slot_map（HashMap<Callable, Slot>）
            ├─ alloca 快照全部 Callable（:1188-1196）  ← 处理函数里断开连接甚至删对象都不影响本轮
            ├─ !callable.is_valid() → 跳过（:1222）    ← 目标已死的连接静默失效
            ├─ CONNECT_DEFERRED → MessageQueue（:1230）
            └─ callable.callp(...)（:1236）
               └─ CallableCustomMethodPointer::call(...)
                  └─ call_with_variant_args(data.instance, data.method, ...)   binder_common.h
                     └─ (data.instance->*data.method)(args...)   ← 真正的调用点
```

参数个数与类型不在 `connect` 时检查，只在发射时由 `ce.error != CALL_OK` 报错（`object.cpp:1239`）。`_on_pressed()` 零参、`pressed` 零参，正好匹配。

三种"看起来该是虚函数却没有 `virtual`"的机制：

| 函数 | 机制 | 绑定时机 |
| --- | --- | --- |
| `SpxUiBinding::_notification` | `GDCLASS` 宏生成的 `_notificationv` 静态调用链（第 7 节） | 编译期宏展开 |
| `BaseButton::_pressed` | `GDVIRTUAL_CALL` + `virtual.gen.py` 生成的覆写钩子表 | 运行期查表，未注册则走默认 |
| `SpxUiBinding::_on_pressed` | `Callable`（对象 ID + 成员函数指针）存进信号槽 | 连接那一刻固化地址 |

三条对本模块代码有实际影响的细节：

1. **`slot_map` 以 Callable 为键**（`object.h:621`）。重复 `connect` 同一个 Callable 不会多出一条槽位：带 `CONNECT_REFERENCE_COUNTED` 才 `reference_count++`，否则直接报 "already connected"（`object.cpp:1446-1452`）。这解释了 `_connect_pressed_first` 里 `while (is_connected(pressed, callable)) { disconnect(...); reference_count++; }` 的动机——**把这条连接的所有引用抽干、记下次数、稍后原样补回**，否则会漏掉被多次计数的连接。
2. **处理器顺序等于连接顺序**。Godot 4.3+ 的 `HashMap` 用 `head_element` / `tail_element` 维护插入序（`hash_map.h:79-80`），`emit_signalp` 按插入序调用，`connect` 总是追加到尾部。所以"先全摘下、连自己的、再按序接回别人的"确实能让自己的槽位落在最前——这就是"插队"技巧的全部依据，也是测试里 `events[0] == 1` 能通过的底层原因。没有优先级字段可用。
3. **两道销毁防线各挡一件事**。`Object::connect` 会把连接登记到**目标对象**的 `connections` 链表（`object.cpp:1464-1466`），`Object::~Object()` 反向断开这些连接（`:2187-2194`）——这一道保证绝不可能调进已释放的 binding。但它挡不住"binding 还活着、`listener` 已失效"，所以 `detach()` 显式断开并清 `listener` 不是冗余；而 `_on_pressed` 里先把 `listener` 与 `gid` 拷进局部变量再调用，挡的是回调执行中途把 listener 换掉。

## 9. 已知风险与待接线处

1. **`set_gid` 必须先于 `set_control_item`**（`spx_ui_mgr.cpp:204-205`，现在顺序是对的）。binding 的 `gid` 是复制来的，顺序颠倒会带着 `gid=0` 绑上去，点击回调静默丢失且难查。这条隐式时序没有断言保护。
2. **布局那一组方法没走防御宏**：`set_rect / set_color / set_font_size / set_visible` 用了 `SPX_UI_CONTROL_LOOKUP_VOID`（宿主空则返回），但 `spx_ui.cpp:338-406` 的 `set_position / set_scale / set_size / set_rotation / get_canvas_scale` 以及 `set_texture` 直接 `get_control()->...`，宿主为空就是空指针解引用。正常情况下"活着的 `SpxUi` 必有活宿主"由 binding 保证，但 `on_spx_ui_destroyed` 开头有一句 `if (!Spx::is_initialized()) return;`（`spx_ui_mgr.cpp:118`）——引擎关闭/重启时序里宿主先死、记录留在 map 中，就会出现"账本还在、宿主没了"的窗口。建议这一组统一走宏，或加 `ERR_FAIL_NULL_V`。
3. **GD 侧点击回调是空实现**：`gdexample/src/GDCallBackHandler.cpp:203` 的 `func_on_ui_clicked` 是空函数体。引擎侧整条探针链路是通的，信号到 GDExtension 层被丢弃——`UiAsk` / `Monitor` 要响应点击，缺的是这一段接线，不是引擎侧。
4. **`SpxUi::set_interactable` 是 `print_line("TODO")` 空壳**，`is_interactable()` 恒 true；`set_font` / `get_font` 同样是空实现。ABI 有槽位、引擎没实现，别在这上面试错。
5. **每个受管 Control 多一个 Node**：这是探针模式的固有成本（内存与树体积）；同时目前只覆盖 7 种控件（`ESpxUiType` + `SpxLabel/SpxImage/...` 那组 typedef 就是 SPX 可见的控件白名单）。
