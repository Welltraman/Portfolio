#include "tptm_next.hpp"
#include "../tpt_handler.hpp"

TPTRegister<TPTMNext> TPTMNext::m_reg("next");

int TPTMNext::_convert_string_enums(const String& a_str) const {
	if (a_str == "ADD") return TPTHandler::ProcessAddMode::ADD;
	else if (a_str == "PAUSE") return TPTHandler::ProcessAddMode::PAUSE;
	else if (a_str == "END") return TPTHandler::ProcessAddMode::END;
	else if (a_str == "LOW") return TPTHandler::Priority::LOW;
	else if (a_str == "NORMAL") return TPTHandler::Priority::NORMAL;
	else if (a_str == "HIGH") return TPTHandler::Priority::HIGH;
	else if (a_str == "IMMEDIATE") return TPTHandler::Priority::IMMEDIATE;
	return -1;
}

bool TPTMNext::_verify_values(Array& r_val) const {
	for (int i = 1; i < r_val.size(); i++) {
		if (r_val[i].get_type() == Variant::STRING) r_val[i] = _convert_string_enums(r_val[i]);
	}
	return r_val.size() == 3 && r_val[0].get_type() == Variant::STRING && r_val[1].get_type() == Variant::INT && r_val[2].get_type() == Variant::INT;
}

void TPTMNext::_on_child_process_end(Ref<TPTProcess> p_process) {
	m_process->end();
}

void TPTMNext::_run() {
	delay = true;
	TPTHandler* handler = (TPTHandler*)Engine::get_singleton()->get_singleton_object("TPT");
	Ref<TPTProcess> process = handler->add_process(m_process->file_name, values[0],
		(TPTHandler::Priority)values[1].operator signed int(),
		(TPTHandler::ProcessAddMode)values[2].operator signed int());
	process->connect("process_ended", this, "_on_child_process_end");
	m_process->child_process = process;
}

Array TPTMNext::get_default_value() const {
	Array array;
	array.append("");
	array.append(TPTHandler::ProcessAddMode::ADD);
	array.append(TPTHandler::Priority::NORMAL);
	return array;
}
