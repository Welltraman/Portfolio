#include "tptm_dialogue.hpp"

TPTRegister<TPTMDialogue> TPTMDialogue::m_reg("dialogue");

bool TPTMDialogue::_verify_values(Array &r_val) const {
	return r_val.size() == 1 && r_val[0].get_type() == Variant::STRING;
}

void TPTMDialogue::_run() {
	ToolsAdapter *adapter = (ToolsAdapter *)Engine::get_singleton()->get_singleton_object("Adapter");
	adapter->call("dialogic", values[0]);
	end();
}

Array TPTMDialogue::get_default_value() const {
	Array array;
	array.push_back("");
	return array;
}
