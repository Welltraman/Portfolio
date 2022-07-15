#pragma once
#include "print_string_extended.hpp"
#include "core/func_ref.h"

class ToolsAdapter : public Object {
	GDCLASS(ToolsAdapter, Object);
	Map<StringName, Ref<FuncRef>> m_adapters;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("register", "func"), &ToolsAdapter::register_adapter);
	}
public:
	static ToolsAdapter* singleton;
	Variant call(const String &a_name, const Variant &a_arg);
	void register_adapter(Ref<FuncRef> p_adapter);
	ToolsAdapter() { singleton = this; }
	~ToolsAdapter() { singleton = nullptr; }
};
