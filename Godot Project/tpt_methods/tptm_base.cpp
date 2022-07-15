#include "tptm_base.hpp"
#include "../tpt_process.hpp"

TPTMethodFactory::Method* TPTMethodFactory::m_method = nullptr;

void TPTMethodBase::end() {
	_end();
	call_deferred("emit_signal", "method_ended", this);
}

void TPTMethodBase::run(TPTProcess *p_process) {
	m_process = p_process;
	if (_verify_values(values) && m_process->is_running())
		_run();
	else end();
}
