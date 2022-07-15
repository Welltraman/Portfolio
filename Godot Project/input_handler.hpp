#pragma once
#include "core/os/keyboard.h"
#include "core/reference.h"
#include "resource_manager.hpp"

class InputHandlerEvent : public Reference {
	GDCLASS(InputHandlerEvent, Reference);
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("say"), &InputHandlerEvent::say);
	}
public:
	virtual void say() {}
};

class InputHandler : public Object {
	GDCLASS(InputHandler, Object);
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("register", "even", "action"), &InputHandler::register_event);
	}
public:
	static InputHandler* singleton;
	void change_keybind(const String& p_name) {}
	void register_event(Ref<InputHandlerEvent> p_e, const StringName& p_action) {}
	void request(const String& p_name) {}
	InputHandler() { singleton = this; }
	~InputHandler() { singleton = nullptr; }
};
