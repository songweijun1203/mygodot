# mygodot

这是一个独立的 Godot + SPX C++ 调试工作区。根目录只保存构建入口、调试配置和测试项目；`godot/` 与 `godot_modules/` 都是源码输入，不会被当前 Git 仓库跟踪。

## 环境

- macOS arm64（默认目标，可通过 `PLATFORM` 和 `ARCH` 覆盖）
- Python 3 和 SCons
- `/usr/bin/clang++`

默认目录结构如下：

```text
mygodot/
├── godot/                 # 独立 Godot checkout，已被 Git 忽略
├── godot_modules/spx/     # SPX module，已被 Git 忽略
├── debug_project/         # 用于启动编辑器和断点调试的最小项目
├── .vscode/               # tasks、LLDB、clangd 配置
├── Makefile
└── .spx-build/            # 生成的 compile_commands.json，已被 Git 忽略
```

## 构建和测试

```sh
make doctor
make build-debug
make test-spx
make run-debug
```

`build-debug` 使用 `target=editor dev_build=yes debug_symbols=true optimize=none tests=yes compiledb=yes`，并通过 `custom_modules` 只加载当前工作区的 SPX module。产物命名为 `godot.macos.editor.dev.arm64.spx-debug`，与 Godot 源码目录中其它构建产物区分开。

只跑某一组测试时可以传 doctest 的通配过滤器：

```sh
make test-spx TEST_CASE_FILTER='*Input proxy*'
```

也可以把 `GODOT_SRC`、`SPX_MODULE_SRC`、`JOBS` 或 `SCONS` 指向其它本地路径：

```sh
make build-debug GODOT_SRC=/path/to/godot SPX_MODULE_SRC=/path/to/godot_modules/spx
```

## C++ 调试

用 VS Code 打开 `spx-cpp.code-workspace`，选择 `SPX: Debug editor` 启动配置即可。该配置会先执行 `make build-debug`，然后用 LLDB 启动自定义编辑器；断点可以直接放在 `godot_modules/spx` 或 `godot` 的 C++ 文件中。`compile_commands.json` 会被复制到 `.spx-build/`，供 clangd 和 C/C++ 扩展索引。

如果已经有可用的 Debug 二进制、只想重新附加调试器而不触发编译，选择 `SPX: Debug editor (no build)`。也可以运行任务 `SPX: run editor without rebuilding`，但它只启动程序，不附加 LLDB。

调试外部 SPX 教程时，打开包含 `SPX` 工作区文件夹的 `spx-cpp.code-workspace`，选择 `SPX: Debug 05-Animation`。它运行 `tutorial/05-Animation/project`，该项目会加载 `project/lib/gdspx-darwin-arm64.dylib`；`SpxSprite` 和 `SpxSpriteMgr` 则来自当前工作区编译的 Godot 模块。重复调试时可选择 `SPX: Debug 05-Animation (no build)`。
