#include "tpt_process.hpp"

void TPTProcess::start() {
	print_raw(vformat("Process %s starting.", get_instance_id()));
	m_state = State::RUNNING;
	m_process_thread = std::thread(&TPTProcess::_call_methods, this, m_methods->front());
}

void TPTProcess::_call_methods(List<Ref<TPTMethodBase>>::Element *p_start) {
	for (List<Ref<TPTMethodBase>>::Element *E = p_start; E; E = E->next()) {
		E->get()->connect("method_ended", this, "_on_method_end");
		E->get()->run(this);
		if (E->get()->delay) m_state = State::DELAYED;
		if (m_state != State::RUNNING) _pause_loop();
		if (m_state == State::ENDED) return;
	}
}

void TPTProcess::_pause_loop() {
	std::unique_lock<std::mutex> ul(m_mutex);
	m_cv.wait(ul, [this] { return m_state == State::RUNNING || m_state == State::ENDED; });
}

void TPTProcess::_on_method_end(const Ref<TPTMethodBase> p_method) {
	print_raw(vformat("Method '%s' ended.", p_method->get_class_name()));
	if (m_state == State::ENDED) return;
	if (m_methods->find(p_method) == m_methods->back()) end();
	else if (p_method->delay) {
		m_state = State::RUNNING;
		m_cv.notify_one();
	}
}

void TPTProcess::add_method_after(Ref<TPTMethodBase> p_method, const Ref<TPTMethodBase> p_after) {
	m_mutex.lock();
	m_methods->insert_after(m_methods->find(p_after), p_method);
	m_mutex.unlock();
}

void TPTProcess::end() {
	m_state = State::ENDED;
	m_cv.notify_one();
	m_process_thread.join();
	emit_signal("process_ended", this);
}

void TPTProcess::force_end() {
	m_state = State::ENDED;
	for (List<Ref<TPTMethodBase>>::Element *E = m_methods->front(); E; E = E->next())
		E->get()->end();
	m_cv.notify_one();
	m_process_thread.join();
	emit_signal("process_ended", this);
}

void TPTProcess::pause() {
	if (m_state != State::DELAYED) m_state = State::PAUSED;
	for (List<Ref<TPTMethodBase>>::Element* E = m_methods->front(); E; E = E->next())
		E->get()->pause();
}

void TPTProcess::resume() {
	for (List<Ref<TPTMethodBase>>::Element* E = m_methods->front(); E; E = E->next())
		E->get()->resume();
	if (m_state == State::PAUSED) {
		m_state = State::RUNNING;
		m_cv.notify_one();
	}
}

TPTProcess::TPTProcess(TPTParser* p_parser, const String& a_filename, const String& a_blockname, const int a_priority)
	: file_name(a_filename), block_name(a_blockname), priority(a_priority), m_state(State::PREPARING) {
	std::thread thread(&TPTProcess::_parse_tpt, this, this, p_parser);
	thread.join();
}

void TPTProcess::_parse_tpt(TPTProcess* p_process, TPTParser* p_parser) {
	p_process->m_mutex.lock();
	p_process->m_methods = p_parser->parse(p_process->file_name, p_process->block_name);
	p_process->m_mutex.unlock();
}
