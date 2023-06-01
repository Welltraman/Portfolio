#pragma once
#include <godot_cpp/classes/node.hpp>
#include "../convertor.hpp"

namespace godot {

class StateCommand : public GDCommand {
    int type = 0;
public:
    _FORCE_INLINE_ int get_type() const { return type; }
    StateCommand(): GDCommand(Callable(), Array()), type(0) {}
    StateCommand(const Callable& p_func, const int p_type): GDCommand(p_func, Array()), type(p_type) {}
};

class StateMachine : public Node {
    GDCLASS(StateMachine, Node);

    enum StateType {
        NIL,
        READY,
        PROCESS,
        PHYSICS,
    };

    MacroCommand* queue = nullptr;
    StateCommand* state = nullptr;

    StringName default_state = "";
    _FORCE_INLINE_ void _set_default(const StringName& p_state) { default_state = p_state; }
    _FORCE_INLINE_ StringName _get_default() const { return default_state; }

    void _initialize();
    void _add_child_states(const MacroCommand* p_queue);
    void _register_state(StateCommand* p_state);

protected:
    void _notification(int p_what);
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_set_default"), &StateMachine::_set_default);
        ClassDB::bind_method(D_METHOD("_get_default"), &StateMachine::_get_default);
        ClassDB::add_property("StateMachine", PropertyInfo(Variant::STRING_NAME, "default"), "_set_default", "_get_default");
        ClassDB::bind_method(D_METHOD("register_state", "callable", "type"), &StateMachine::register_state, DEFVAL(READY));
        ClassDB::bind_method(D_METHOD("set_state", "id", "arg"), &StateMachine::set_state, DEFVAL(Array()));
        ClassDB::bind_method(D_METHOD("get_state"), &StateMachine::get_state);

        BIND_ENUM_CONSTANT(READY);
        BIND_ENUM_CONSTANT(PROCESS);
        BIND_ENUM_CONSTANT(PHYSICS);
    }

public:
    _FORCE_INLINE_ void register_state(const Callable& p_callable, const StateType p_type = READY) {
        _register_state(new StateCommand(p_callable, p_type));
    }
    _FORCE_INLINE_ void set_state(const StringName& p_id, const Array& p_args = Array()) {
        if (queue) {
            state = (StateCommand*)queue->find(p_id);
            if (state) {
                state->args = p_args;
            } else {
                state = new StateCommand;
            }
        } else {
            state = new StateCommand;
        }
    }
    _FORCE_INLINE_ StringName get_state() const { return (state) ? state->get_id() : ""; }

    ~StateMachine() {
        delete state;
        delete queue;
    }
};

}

VARIANT_ENUM_CAST(StateMachine::StateType);