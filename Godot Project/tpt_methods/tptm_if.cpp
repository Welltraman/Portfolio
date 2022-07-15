#include "tptm_if.hpp"
#include "tptm_next.hpp"
#include "../tpt_handler.hpp"

TPTRegister<TPTMIf> TPTMIf::m_reg("if");

bool TPTMIf::_verify_values(Array &r_val) const {
	return r_val.size() == 2 && (r_val[0].get_type() == Variant::BOOL || r_val[0].get_type() == Variant::STRING) && r_val[1].get_type() == Variant::STRING;
}

void TPTMIf::_run() {
	if (!values[0].operator bool()) {
		Ref<TPTMethodBase> next = TPTMethodFactory::get("next");
		Array array;
		array.push_back(values[1]);
		array.push_back(0);
		array.push_back(1);
		next->values = array;
		m_process->add_method_after(next, this);
	}
	end();
}

Array TPTMIf::get_default_value() const {
	Array array;
	array.push_back("");
	array.push_back("");
	return array;
}
