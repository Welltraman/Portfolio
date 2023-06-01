#pragma once
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

class Command {
public:
    _FORCE_INLINE_ virtual StringName get_id() const = 0;
    _FORCE_INLINE_ virtual Variant execute() = 0;
};

class GDCommand : public Command {
protected:
    Callable callable = Callable();
    
public:
    Array args = Array();

    _FORCE_INLINE_ virtual int64_t get_owner_id() const { return callable.get_object_id(); }
    _FORCE_INLINE_ StringName get_id() const override { return callable.get_method(); }
    _FORCE_INLINE_ Variant execute() override { return callable.callv(args); }
    GDCommand(const Callable& p_func, const Array& p_arg): callable(p_func), args(p_arg) {}
};

class MacroCommand : public Command {
protected:
    List<Command*> commands = List<Command*>();
    Array ret_values = Array();

public:
    _FORCE_INLINE_ StringName get_id() const override { return _STR(MacroCommand); }

    Variant execute() override {
        for (List<Command*>::Element* E = commands.front(); E; E = E->next()) {
            ret_values.push_back(E->get()->execute());
        }
        return ret_values;
    }

    _FORCE_INLINE_ virtual void add(Command* p_cmd, List<Command*>::Element* p_after = nullptr) {
        if (p_after) {
            commands.insert_after(p_after, p_cmd);
        } else {
            commands.push_back(p_cmd);
        }
    }

    virtual void remove(const List<Command*>::Element* p_elem) {
        delete p_elem->get();
        commands.erase(p_elem->get());
    }

    virtual Command* find(const StringName& p_id) {
        for (List<Command*>::Element* E = commands.front(); E; E = E->next()) {
            if (E->get()->get_id() == p_id) {
                return E->get();
            }
        }
        return nullptr;
    }

    _FORCE_INLINE_ virtual void flush() { 
        for (List<Command*>::Element* E = commands.front(); E; E = E->next()) {
            remove(E);
        }
        commands.clear();
    }

    _FORCE_INLINE_ const List<Command*>& get_list() const { return commands; }

    _FORCE_INLINE_ int size() const { return commands.size(); }

    ~MacroCommand() { flush(); }
};

}