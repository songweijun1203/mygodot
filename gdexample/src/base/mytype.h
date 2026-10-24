#pragma once
#include <cstdint>

using Object_t = int64_t;

struct XVec2{
	float x{};
	float y{};
};

struct XRect2{
	XVec2 position{};
	XVec2 size{};
};


// Godot Key 的特殊键编码：SPECIAL = 1 << 22。
// SpxInputProxy 会把方向键转换为这些值后传入回调。
enum class Keys : int64_t {
	left = (1LL << 22) | 0x0F,  // Godot Key::LEFT  = 0x40000F = 4194319
	up = (1LL << 22) | 0x10,    // Godot Key::UP    = 0x400010 = 4194320
	right = (1LL << 22) | 0x11, // Godot Key::RIGHT = 0x400011 = 4194321
	down = (1LL << 22) | 0x12,  // Godot Key::DOWN  = 0x400012 = 4194322
	space = 0x20                // Godot Key::SPACE = 0x20 = 32
};

enum class MouseKey:int64_t{
	none =0,
	left=1,
	right=2,
	middle=4
};

enum class PhysicsMode {
    NoPhysics = 0,
    Kinematic = 1,
    Dynamic = 2,
    Static = 3,
};

struct Matrix3f{
	float colo[3];
	float col1[3];
	float col2[3];
};
