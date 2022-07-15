#include "tptm_wait.hpp"

TPTRegister<TPTMWait> TPTMWait::m_reg("wait");

void TPTMWait::_end() {
	m_timer->release_connections();
}

void TPTMWait::_on_timeout() {
	end();
}

void TPTMWait::_pause() {
	m_pause_time_left = m_timer->get_time_left();
	m_timer->release_connections();
}

void TPTMWait::_resume() {
	m_timer = SceneTree::get_singleton()->create_timer(m_pause_time_left);
	m_timer->connect("timeout", this, "_on_timeout");
}

bool TPTMWait::_verify_values(Array& r_val) const {
	return r_val.size() == 1 && (r_val[0].get_type() == Variant::REAL || r_val[0].get_type() == Variant::INT);
}

void TPTMWait::_run() {
	m_timer = SceneTree::get_singleton()->create_timer(values[0]);
	m_timer->connect("timeout", this, "_on_timeout");
}

Array TPTMWait::get_default_value() const {
	Array array;
	array.push_back(NULL);
	return array;
}
