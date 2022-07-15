#pragma once
#include "tptm_base.hpp"

class TPTMIf : public TPTMethodBase {
	GDCLASS(TPTMIf, TPTMethodBase);
	static TPTRegister<TPTMIf> m_reg;
	void _end() {}
	void _pause() {}
	void _resume() {}
	void _run();
	bool _verify_values(Array& r_val) const;
public:
	Array get_default_value() const;
};
