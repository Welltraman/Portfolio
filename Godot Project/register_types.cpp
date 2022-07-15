#include "core/project_settings.h"
#include "config_manager.hpp"
#include "config_resource.hpp"
#include "input_handler.hpp"
#include "json_resource.hpp"
#include "resource_manager.hpp"
#include "text_file_resource.hpp"
#include "tools_adapter.hpp"
#include "tpt_handler.hpp"
#include "tpt_process.hpp"
#include "tpt_resource.hpp"
#include "tpt_methods/tptm_base.hpp"
#include "tpt_methods/tptm_debug.hpp"
#include "tpt_methods/tptm_dialogue.hpp"
#include "tpt_methods/tptm_if.hpp"
#include "tpt_methods/tptm_next.hpp"
#include "tpt_methods/tptm_repeat.hpp"
#include "tpt_methods/tptm_wait.hpp"

static Ref<ResourceFormatLoaderConfig> config_loader;
static Ref<ResourceFormatSaverConfig> config_saver;
static Ref<ResourceFormatLoaderJSON> json_loader;
static Ref<ResourceFormatSaverJSON> json_saver;
static Ref<ResourceFormatLoaderTextFile> text_file_loader;
static Ref<ResourceFormatSaverTextFile> text_file_saver;
static Ref<ResourceFormatLoaderTPT> tpt_loader;
static Ref<ResourceFormatSaverTPT> tpt_saver;

static ConfigManager *config_manager = ConfigManager::singleton;
static InputHandler *input_handler = InputHandler::singleton;
static ResourceManager* resource_manager = ResourceManager::singleton;
static ToolsAdapter* tools_adapter = ToolsAdapter::singleton;
static TPTHandler* tpt_handler = TPTHandler::singleton;

void register_singletons() {
	tools_adapter = memnew(ToolsAdapter);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Adapter", tools_adapter));

	resource_manager = memnew(ResourceManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("RM", resource_manager));

	config_manager = memnew(ConfigManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Config", config_manager));

	tpt_handler = memnew(TPTHandler);
	Engine::get_singleton()->add_singleton(Engine::Singleton("TPT", tpt_handler));

	input_handler = memnew(InputHandler);
	Engine::get_singleton()->add_singleton(Engine::Singleton("InputHandler", input_handler));
}

void register_settings() {
	GLOBAL_DEF(RESOURCE_MANAGER_SETTING, Dictionary());
	GLOBAL_DEF(RESOURCE_MANAGER_DIRECTORY_SETTING + "/tpt", "");
}

template <class TL, class TS>
void _add_resource(Ref<TL>& r_loader, Ref<TS>& r_saver) {
	r_loader.instance();
	ResourceLoader::add_resource_format_loader(r_loader);
	r_saver.instance();
	ResourceSaver::add_resource_format_saver(r_saver);
}

void register_resources() {
	_add_resource(text_file_loader, text_file_saver);
	_add_resource(config_loader, config_saver);
	_add_resource(json_loader, json_saver);
	_add_resource(tpt_loader, tpt_saver);
}

void register_editor_plugins() {
	EditorPlugins::add_by_type<ResourceManagerPlugin>();
}

void register_tpt_methods() {
	ClassDB::register_class<TPTMethodBase>();
	ClassDB::register_class<TPTMDebug>();
	ClassDB::register_class<TPTMDialogue>();
	ClassDB::register_class<TPTMIf>();
	ClassDB::register_class<TPTMNext>();
	ClassDB::register_class<TPTMRepeat>();
	ClassDB::register_class<TPTMWait>();
}

void register_tools_types() {
	ClassDB::register_class<ToolsAdapter>();
	ClassDB::register_class<ResourceManager>();
	ClassDB::register_class<ConfigManager>();

	ClassDB::register_class<ConfigResource>();
	ClassDB::register_class<JSONResource>();
	ClassDB::register_class<TextFileResource>();
	ClassDB::register_class<TPTResource>();

	ClassDB::register_class<TPTProcess>();
	ClassDB::register_class<TPTHandler>();

	ClassDB::register_class<InputHandler>();
	ClassDB::register_class<InputHandlerEvent>();

	register_resources();
	register_tpt_methods();
	register_singletons();
	register_settings();
#ifdef TOOLS_ENABLED
	register_editor_plugins();
#endif
}

template <class TL, class TS>
void _remove_resource(Ref<TL>& r_loader, Ref<TS>& r_saver) {
	ResourceLoader::remove_resource_format_loader(r_loader);
	r_loader.unref();
	ResourceSaver::remove_resource_format_saver(r_saver);
	r_saver.unref();
}

void unregister_tools_types() {
	_remove_resource(tpt_loader, tpt_saver);
	_remove_resource(json_loader, json_saver);
	_remove_resource(config_loader, config_saver);
	_remove_resource(text_file_loader, text_file_saver);

	memdelete(input_handler);
	memdelete(tpt_handler);
	memdelete(config_manager);
	memdelete(resource_manager);
	memdelete(tools_adapter);
}
