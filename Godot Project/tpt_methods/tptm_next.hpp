#pragma once
#include "tptm_base.hpp"

class TPTProcess;
class TPTMNext : public TPTMethodBase {
	GDCLASS(TPTMNext, TPTMethodBase);
	static TPTRegister<TPTMNext> m_reg;
	int _convert_string_enums(const String& a_str) const;
	void _end() {}
	void _on_child_process_end(Ref<TPTProcess> p_process);
	void _pause() {}
	void _resume() {}
	bool _verify_values(Array& r_val) const;
	void _run();
protected:
	static void _bind_methods() {
		ClassDB::bind_method("_on_child_process_end", &TPTMNext::_on_child_process_end);
	}
public:
	Array get_default_value() const;
};
