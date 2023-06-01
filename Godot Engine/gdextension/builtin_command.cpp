#include "builtin_command.hpp"

using namespace godot;

Variant BuiltinCommand::_set_property(const String& p_path, const Variant& p_var) {
    PackedStringArray arr = p_path.split(":", false);
    Object* target = Convertor::parse(arr[0]);
    if (target && arr.size() == 2) {
        target->set_indexed(arr[1], p_var);
        return target->get_indexed(arr[1]);
    }
    return Variant();
}

Variant BuiltinCommand::_call_func(const String& p_path, const Array& p_arg) {
    PackedStringArray arr = p_path.split(":", false); // func name should follow collon
    Object* target = Convertor::parse(arr[0]);
    if (target && arr.size() == 2) {
        return target->callv(arr[1], p_arg);
    }
    return Variant();
}