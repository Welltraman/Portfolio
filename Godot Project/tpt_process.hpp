#pragma once
#include "tpt_parser.hpp"

class TPTProcess : public Reference {
	GDCLASS(TPTProcess, Reference);

	enum class State {
		PREPARING,
		RUNNING,
		DELAYED,
		PAUSED,
		ENDED,
	} m_state;

	List<Ref<TPTMethodBase>>* m_methods = nullptr;
	std::thread m_process_thread;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	void _call_methods(List<Ref<TPTMethodBase>>::Element* p_start);
	void _on_method_end(const Ref<TPTMethodBase> p_method);
	void _parse_tpt(TPTProcess* p_process, TPTParser* p_parser);
	void _pause_loop();
protected:
	static void _bind_methods() {
		ClassDB::bind_method("_on_method_end", &TPTProcess::_on_method_end);
		ClassDB::bind_method("end", &TPTProcess::end);
		ClassDB::bind_method("pause", &TPTProcess::pause);
		ClassDB::bind_method("resume", &TPTProcess::resume);
		ADD_SIGNAL(MethodInfo("process_ended", PropertyInfo(Variant::OBJECT, "tpt_process")));
	}
public:
	String file_name = "";
	String block_name = "";
	const int priority;
	Ref<TPTProcess> child_process = NULL;
	void add_method_after(Ref<TPTMethodBase> p_method, const Ref<TPTMethodBase> p_after);
	List<Ref<TPTMethodBase>> *get_methods() const { return m_methods; }
	void end();
	void force_end();
	void pause();
	void resume();
	void start();
	bool is_running() { return ((m_state != State::PREPARING) || (m_state != State::PAUSED) || (m_state != State::ENDED)); }
	TPTProcess(TPTParser* p_parser = nullptr, const String& a_filename = "", const String& a_blockname = "", const int a_priority = 0);
};
