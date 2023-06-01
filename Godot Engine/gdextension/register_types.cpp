#include "register_types.h"
#include "builtin_command.hpp"
#include "cmd/command_prompt.hpp"
#include "debug/debug_draw.hpp"
#include "state/state_machine.hpp"
#include "tpt/tpt_agent.hpp"
#include "tpt/tpt_method.hpp"

using namespace godot;

static CommandRegister* cmd_reg = CommandRegister::get_singleton();
static BuiltinCommand* cmd = BuiltinCommand::get_singleton();
static TPTMethods* tpt_mthd = TPTMethods::get_singleton();

void initialize_utils_module(ModuleInitializationLevel p_level) {
    switch (p_level) {
        case MODULE_INITIALIZATION_LEVEL_CORE: {
        } break;
        case MODULE_INITIALIZATION_LEVEL_SERVERS: {
            
            ClassDB::register_class<CommandRegister>();
            ClassDB::register_class<BuiltinCommand>();
            ClassDB::register_class<TPTMethods>();

        } break;
        case MODULE_INITIALIZATION_LEVEL_SCENE: {

            cmd_reg = memnew(CommandRegister);
            Engine::get_singleton()->register_singleton("Command", cmd_reg);

            cmd = memnew(BuiltinCommand);
            CommandRegister::get_singleton()->register_factory(Callable(cmd, SET_PROPERTY_ID));
            CommandRegister::get_singleton()->register_factory(Callable(cmd, CALL_FUNC_ID));

            tpt_mthd = memnew(TPTMethods);
            CommandRegister::get_singleton()->register_factory(Callable(tpt_mthd, String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_DELAY).c_str())));
            CommandRegister::get_singleton()->register_factory(Callable(tpt_mthd, String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_NEXT).c_str())));
            CommandRegister::get_singleton()->register_factory(Callable(tpt_mthd, String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_IF).c_str())));
            CommandRegister::get_singleton()->register_factory(Callable(tpt_mthd, String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_BEGIN).c_str())));
            CommandRegister::get_singleton()->register_factory(Callable(tpt_mthd, String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_END).c_str())));

            ClassDB::register_class<DebugDraw>();
            ClassDB::register_class<CommandPrompt>();
            ClassDB::register_class<StateMachine>();
            ClassDB::register_class<TPTAgent>();
            
        } break;
        case MODULE_INITIALIZATION_LEVEL_EDITOR: {
        } break;
    }
}

void uninitialize_utils_module(ModuleInitializationLevel p_level) {
    switch (p_level) {
        case MODULE_INITIALIZATION_LEVEL_CORE: {
        } break;
        case MODULE_INITIALIZATION_LEVEL_SERVERS: {
        } break;
        case MODULE_INITIALIZATION_LEVEL_SCENE: {
            memdelete(tpt_mthd);
            memdelete(cmd_reg);
        } break;
        case MODULE_INITIALIZATION_LEVEL_EDITOR: {
        } break;
    }
}

extern "C" {
GDExtensionBool GDE_EXPORT utils_library_init(const GDExtensionInterface *p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

    init_obj.register_initializer(initialize_utils_module);
    init_obj.register_terminator(uninitialize_utils_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

    return init_obj.init();
}
}