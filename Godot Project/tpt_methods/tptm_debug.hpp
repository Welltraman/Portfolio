#pragma once
#include "tptm_base.hpp"

class TPTMDebug : public TPTMethodBase {
	GDCLASS(TPTMDebug, TPTMethodBase);
	static TPTRegister<TPTMDebug> m_reg;
	void _end() {}
	void _pause() {}
	void _resume() {}
	void _run();
	bool _verify_values(Array& r_val) const;
public:
	Array get_default_value() const;
};
