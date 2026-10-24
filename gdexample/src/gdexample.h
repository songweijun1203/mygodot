#pragma once

#include "spx_api.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/string.hpp>


namespace godot {

class GdExample : public Node2D {
	GDCLASS(GdExample, Node2D);

private:
	String message = "Hello from C++ GDExtension";
	spx_api::ObjectId spx_sprite_id = 0;

protected:
	static void _bind_methods();

public:
	GdExample() = default;
	~GdExample() = default;

	void _ready() override;
	void _draw() override;

	void set_message(const String &p_message);
	String get_message() const;
};

} // namespace godot
