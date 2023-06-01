#include "state_machine.hpp"

using namespace godot;

void StateMachine::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_READY: {
            set_process(true);
            set_physics_process(true);
            _initialize();
            if (state->get_type() == READY) {
                state->execute();
            }
        } break;
        case NOTIFICATION_PROCESS: {
            if (state->get_type() == PROCESS) {
                state->execute();
            }
        } break;
        case NOTIFICATION_PHYSICS_PROCESS: {
            if (state->get_type() == PHYSICS) {
                state->execute();
            }
        } break;
    }
}

void StateMachine::_initialize() {
    for (int i = 0; i < get_child_count(); i++) {
        if (get_child(i)->get_class() == _STR(StateMachine)) {
            StateMachine* sm = (StateMachine*)get_child(i);
            _add_child_states(sm->queue);
        }
    }

    set_state(default_state);
}

void StateMachine::_add_child_states(const MacroCommand* p_queue) {
    for (const List<Command*>::Element* E = p_queue->get_list().front(); E; E = E->next()) {
        _register_state((StateCommand*)E->get());
    }
}

void StateMachine::_register_state(StateCommand* p_state) {
    if (!queue) {
        queue = new MacroCommand;
    }
    for (const List<Command*>::Element* E = queue->get_list().front(); E; E = E->next()) {
        if (E->get()->get_id() == p_state->get_id() && ((StateCommand*)E->get())->get_type() == p_state->get_type()) {
            delete p_state;
            return;
        }
    }
    queue->add(p_state);
}