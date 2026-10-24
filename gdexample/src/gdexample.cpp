#include "gdexample.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void GdExample::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_message", "message"), &GdExample::set_message);
	ClassDB::bind_method(D_METHOD("get_message"), &GdExample::get_message);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "message"), "set_message", "get_message");
}

void GdExample::_ready() {
	auto *message_label = memnew(Label);
	message_label->set_name("Message");
	message_label->set_text(message);
	message_label->set_position(Vector2(-160, 70));
	message_label->set_size(Vector2(320, 32));
	message_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
	add_child(message_label);

	UtilityFunctions::print("GdExample ready: ", message);
	queue_redraw();
}

void GdExample::_draw() {
	draw_circle(Vector2(), 48.0, Color(0.20, 0.70, 1.00));
	draw_line(Vector2(-64, 0), Vector2(64, 0), Color(1.0, 1.0, 1.0), 2.0);
	draw_line(Vector2(0, -64), Vector2(0, 64), Color(1.0, 1.0, 1.0), 2.0);
}

void GdExample::set_message(const String &p_message) {
	message = p_message;
}

String GdExample::get_message() const {
	return message;
}
