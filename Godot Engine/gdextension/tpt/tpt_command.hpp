#pragma once
#include "../command_factory.hpp"
#include "tpt_tokenizer.hpp"

namespace godot {

class TPTMacro : public MacroCommand {
    List<Command*>::Element* current = nullptr;

public:
    _FORCE_INLINE_ StringName get_id() const override { return _STR(TPTMacro); }

    Variant execute() override {
        if (!current) {
            current = commands.front();
        } else {
            current = current->next();
            if (!current) return NULL;
        }
        return current->get()->execute();
    }

    _FORCE_INLINE_ List<Command*>::Element* get_current() const { return current; }
};

}