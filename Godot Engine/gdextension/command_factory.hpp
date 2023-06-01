#pragma once
#include <godot_cpp/core/class_db.hpp>
#include "convertor.hpp"

namespace godot {

class GDCommandFactory: Factory<GDCommand, Array> {
    Callable callable = Callable();
public:
    _FORCE_INLINE_ GDCommand* create(const Array& p_arr) override { return new GDCommand(callable, p_arr); }
    GDCommandFactory(const Callable& p_callable): callable(p_callable) {
        String id = p_callable.get_method();
        this->get_factory().insert({ id.utf8().get_data(), this });
    }
};

class CommandRegister : public Object {
    GDCLASS(CommandRegister, Object);
    inline static CommandRegister* singleton = nullptr;
protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("register", "callable"), &CommandRegister::register_factory);
        ClassDB::bind_method(D_METHOD("parse", "command"), &CommandRegister::parse_command);
    }
public:
    _FORCE_INLINE_ static CommandRegister* get_singleton() { return singleton; }
    _FORCE_INLINE_ void register_factory(const Callable& p_callable) { new GDCommandFactory(p_callable); }
    _FORCE_INLINE_ Variant parse_command(const String& p_cmd_line) { return Convertor::parse(p_cmd_line); }
    CommandRegister() { singleton = this; }
    ~CommandRegister() { singleton = nullptr; }
};

}