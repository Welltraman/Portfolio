#pragma once
#include "command_factory.hpp"

namespace godot {

const static char* SET_PROPERTY_ID = "setc";
const static char* CALL_FUNC_ID = "callc";

class BuiltinCommand : public Object {
    GDCLASS(BuiltinCommand, Object);

    Variant _set_property(const String& p_path, const Variant& p_var);
    Variant _call_func(const String& p_path, const Array& p_arg);

    inline static BuiltinCommand* singleton = nullptr;

protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD(SET_PROPERTY_ID), &BuiltinCommand::_set_property);
        ClassDB::bind_method(D_METHOD(CALL_FUNC_ID), &BuiltinCommand::_call_func);
    }

public:
    static BuiltinCommand* get_singleton() { return singleton; }
    BuiltinCommand() { singleton = this; }
    ~BuiltinCommand() { singleton = nullptr; }
};

}