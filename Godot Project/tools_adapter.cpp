#pragma once
#include "tools_adapter.hpp"

ToolsAdapter *ToolsAdapter::singleton = nullptr;

Variant ToolsAdapter::call(const String& a_name, const Variant& a_arg) {
	const Variant *ptr = &a_arg;
	Variant::CallError err;
	return m_adapters[a_name]->call_func(&ptr, 1, err);
}

void ToolsAdapter::register_adapter(Ref<FuncRef> p_adapter) {
	m_adapters.insert(p_adapter->get_function(), p_adapter);
}
