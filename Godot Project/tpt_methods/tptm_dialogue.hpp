#pragma once
#include "tptm_base.hpp"
#include "../tools_adapter.hpp"

class TPTMDialogue : public TPTMethodBase {
	GDCLASS(TPTMDialogue, TPTMethodBase);
	static TPTRegister<TPTMDialogue> m_reg;
	void _end() {}
	void _pause() {}
	void _resume() {}
	void _run();
	bool _verify_values(Array& r_val) const;
public:
	Array get_default_value() const;
};
