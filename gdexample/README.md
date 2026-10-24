# gdexample

This is a minimal Godot 4 C++ GDExtension using the local `godot-cpp` 4.4
bindings.

## Build

From this directory:

```bash
scons platform=macos target=template_debug arch=arm64 debug_symbols=yes optimize=debug
```

The library is written to:

```text
project/bin/libgdexample.macos.template_debug.dylib
```

## Run

```bash
godot --path project --editor
```

Use the Godot binary that matches the `godot-cpp` branch. The current binding
checkout is the `4.4` branch, so the Godot runtime should also be Godot 4.4.
