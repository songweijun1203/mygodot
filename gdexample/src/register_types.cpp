#include "gdexample.h"
#include "register_types.h"
#include "spx_api.h"
#include "GDCallBackHandler.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_gdexample(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(GdExample);
}

void uninitialize_gdexample(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {

GDExtensionBool GDE_EXPORT gdexample_library_init(
		GDExtensionInterfaceGetProcAddress p_get_proc_address,
		GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(
			p_get_proc_address, p_library, r_initialization);

	auto &spx = spx_api::GDExtensionInterface::get_instance();
	spx.initialize(p_get_proc_address);
	if (!spx) {
		return false;
	}

	auto s = GDCallBackHandler::build_callback_info();
	spx.global_register_callbacks((spx_api::GDExtensionSpxCallbackInfoPtr)&s);


	// 注册GdExample类
	init_obj.register_initializer(initialize_gdexample);
	init_obj.register_terminator(uninitialize_gdexample);
	init_obj.set_minimum_library_initialization_level(
			MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}

} // extern "C"
