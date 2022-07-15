#pragma once
#include "tptm_base.hpp"
#include "scene/main/scene_tree.h"

class TPTMWait : public TPTMethodBase {
	GDCLASS(TPTMWait, TPTMethodBase);
	static TPTRegister<TPTMWait> m_reg;
	Ref<SceneTreeTimer> m_timer;
	float m_pause_time_left = 0;
	void _end();
	void _pause();
	void _resume();
	bool _verify_values(Array& r_val) const;
	void _on_timeout();
	void _run();
protected:
	static void _bind_methods() {
		ClassDB::bind_method("_on_timeout", &TPTMWait::_on_timeout);
	}
public:
	Array get_default_value() const;
};
