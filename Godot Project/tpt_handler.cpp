#include "tpt_handler.hpp"

TPTHandler* TPTHandler::singleton = nullptr;

Ref<TPTProcess> TPTHandler::add_process(const String &a_filename, const String &a_blockname, const Priority a_priority, const ProcessAddMode a_mode) {
	Ref<TPTProcess> process = memnew(TPTProcess(&m_parser, a_filename, a_blockname, a_priority));
	process->connect("process_ended", this, "_on_process_end");
	_start_process(_add_process(process), a_mode);
	return process;
}

int TPTHandler::_add_process(const Ref<TPTProcess> a_process) {
	for (int i = 0; i < m_active_tpts.size(); i++) {
		if (m_active_tpts[i]->priority < a_process->priority) {
			m_active_tpts.insert_before(m_active_tpts.find(m_active_tpts[i]), a_process);
			return i;
		}
	}
	m_active_tpts.push_back(a_process);
	return m_active_tpts.size() - 1;
}

void TPTHandler::_start_process(const int a_index, const ProcessAddMode a_add_mode) {
	switch (a_add_mode) {
	case PAUSE:
		call_deferred("_pause_process", a_index + 1);
		break;
	case END:
		call_deferred("_end_process", a_index + 1);
		break;
	}
	m_active_tpts[a_index]->start();
}

void TPTHandler::_end_process(const int a_index) {
	for (int i = a_index; i < m_active_tpts.size(); i++) {
		if (m_active_tpts[i]->priority < m_active_tpts[a_index - 1]->priority && m_active_tpts[i]->child_process == NULL) {
			m_active_tpts[i]->force_end();
		}
	}
}

void TPTHandler::_pause_process(const int a_index) {
	for (int i = a_index; i < m_active_tpts.size(); i++) {
		if (m_active_tpts[i]->priority < m_active_tpts[a_index - 1]->priority && m_active_tpts[i]->child_process == NULL) {
			m_active_tpts[i]->pause();
		}
	}
}

void TPTHandler::_resume_process(const int a_index) {
	for (int i = a_index; i < m_active_tpts.size(); i++)
		m_active_tpts[i]->resume();
}

void TPTHandler::_on_process_end(const Ref<TPTProcess> a_process) {
	print_raw(vformat("Process %s ended.", a_process->get_instance_id()));
	m_active_tpts.erase(a_process);
}
