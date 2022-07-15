#pragma once
#include "tptm_base.hpp"

class TPTMRepeat : public TPTMethodBase {
	GDCLASS(TPTMRepeat, TPTMethodBase);
	static TPTRegister<TPTMRepeat> m_reg;
	void _beginning_repeat();
	void _closing_repeat();
	Ref<TPTMethodBase> _create_copy(const Ref<TPTMethodBase> p_base);
	Ref<TPTMRepeat> _get_repeat_pair();
	void _end() {}
	void _pause() {}
	void _resume() {}
	void _run();
	bool _verify_values(Array& r_val) const;
public:
	Ref<TPTMRepeat> pair;
	int repeat_amount = 0;
	Array get_default_value() const;
};
