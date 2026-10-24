#include "GDCallBackHandler.h"
#include "gdexample.h"
#include "DataManger.h"
#include "engine/Engine.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/node_path.hpp>

#include "iostream"

void GDCallBackHandler::func_on_engine_start()
{
	auto *engine = godot::Engine::get_singleton();
	if (engine == nullptr)
	{
		return;
	}

	auto *tree = godot::Object::cast_to<godot::SceneTree>(engine->get_main_loop());
	if (tree == nullptr || tree->get_current_scene() == nullptr)
	{
		return;
	}

	auto *window = tree->get_root();
	auto *spx_root = window->get_node_or_null(
		godot::NodePath("SpxEngineNode"));

	DataManger::getInstance().init();

	DataManger::getInstance().getEngine()->OnEngineStart(spx_root);
}

void GDCallBackHandler::func_on_engine_update(float delta)
{
	DataManger::getInstance().getEngine()->OnEngineUpdate(delta);
}

void GDCallBackHandler::func_on_engine_fixed_update(float delta)
{
	DataManger::getInstance().getEngine()->OnEngineFixedUpdate(delta);
}

void GDCallBackHandler::func_on_engine_destroy()
{
}

void GDCallBackHandler::func_on_engine_destroyed()
{
}

void GDCallBackHandler::func_on_engine_reset()
{
}

void GDCallBackHandler::func_on_engine_pause(spx_api::GdBool is_paused)
{
}

void GDCallBackHandler::func_on_scene_sprite_instantiated(spx_api::GdInt obj, spx_api::GdString type_name)
{
}

void GDCallBackHandler::func_on_sprite_ready(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_updated(spx_api::GdFloat delta)
{
}

void GDCallBackHandler::func_on_sprite_fixed_updated(spx_api::GdFloat delta)
{
}

void GDCallBackHandler::func_on_sprite_destroyed(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_frames_set_changed(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_animation_changed(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_frame_changed(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_animation_looped(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_animation_finished(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_vfx_finished(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_sprite_screen_exited(spx_api::GdInt obj)
{
	DataManger::getInstance().getEngine()->OnSpriteScreenExited(obj);
}

void GDCallBackHandler::func_on_sprite_screen_entered(spx_api::GdInt obj)
{
	DataManger::getInstance().getEngine()->OnSpriteScreenEntered(obj);
}

void GDCallBackHandler::func_on_mouse_pressed(spx_api::GdInt keyid)
{
}

void GDCallBackHandler::func_on_mouse_released(spx_api::GdInt keyid)
{
}

void GDCallBackHandler::func_on_key_pressed(spx_api::GdInt keyid)
{
	DataManger::getInstance().getEngine()->OnKeyPressed(keyid);
}

void GDCallBackHandler::func_on_key_released(spx_api::GdInt keyid)
{
	DataManger::getInstance().getEngine()->OnKeyReleased(keyid);
}

void GDCallBackHandler::func_on_action_pressed(spx_api::GdString action_name)
{
}

void GDCallBackHandler::func_on_action_just_pressed(spx_api::GdString action_name)
{
}

void GDCallBackHandler::func_on_action_just_released(spx_api::GdString action_name)
{
}

void GDCallBackHandler::func_on_axis_changed(spx_api::GdString action_name, spx_api::GdFloat value)
{
}

void GDCallBackHandler::func_on_collision_enter(spx_api::GdInt self_id, spx_api::GdInt other_id)
{
	DataManger::getInstance().getEngine()->OnSpriteCollisionEnter(self_id, other_id);
}

void GDCallBackHandler::func_on_collision_stay(spx_api::GdInt self_id, spx_api::GdInt other_id)
{
}

void GDCallBackHandler::func_on_collision_exit(spx_api::GdInt self_id, spx_api::GdInt other_id)
{
}

void GDCallBackHandler::func_on_trigger_enter(spx_api::GdInt self_id, spx_api::GdInt other_id)
{
	DataManger::getInstance().getEngine()->OnSpriteTriggerEnter(self_id, other_id);
}
void GDCallBackHandler::func_on_trigger_stay(spx_api::GdInt self_id, spx_api::GdInt other_id)
{
}

void GDCallBackHandler::func_on_trigger_exit(spx_api::GdInt self_id, spx_api::GdInt other_id)
{
	DataManger::getInstance().getEngine()->OnSpriteTriggerExited(self_id, other_id);
}

void GDCallBackHandler::func_on_ui_ready(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_updated(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_destroyed(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_pressed(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_released(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_hovered(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_clicked(spx_api::GdInt obj)
{
}

void GDCallBackHandler::func_on_ui_toggle(spx_api::GdInt obj, spx_api::GdBool is_toggled)
{
}

void GDCallBackHandler::func_on_ui_text_changed(spx_api::GdInt obj, spx_api::GdString text)
{
}
