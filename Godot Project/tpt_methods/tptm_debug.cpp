#include "tptm_debug.hpp"

TPTRegister<TPTMDebug> TPTMDebug::m_reg("debug");

bool TPTMDebug::_verify_values(Array& r_val) const {
	return r_val.size() == 1 && r_val[0].get_type() == Variant::STRING;
}

void TPTMDebug::_run() {
	print_line(values[0].operator String());
	end();
}

Array TPTMDebug::get_default_value() const {
	Array array;
	array.push_back("test");
	return array;
}
