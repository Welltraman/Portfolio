#pragma once
#include "tpt_process.hpp"

class TPTHandler : public Object {
	GDCLASS(TPTHandler, Object);

public:
	enum Priority {
		LOW = 0,
		NORMAL,
		HIGH,
		IMMEDIATE,
	};
	enum ProcessAddMode {
		ADD,
		PAUSE,
		END,
	};

private:
	List<Ref<TPTProcess>> m_active_tpts;
	TPTParser m_parser;
	int _add_process(const Ref<TPTProcess> p_process);
	void _end_process(const int a_index);
	void _on_process_end(const Ref<TPTProcess> a_process);
	void _pause_process(const int a_index);
	void _resume_process(const int a_index);
	void _start_process(const int a_index, const ProcessAddMode a_add_mode);
protected:
	static void _bind_methods() {
		ClassDB::bind_method("_on_process_end", &TPTHandler::_on_process_end);
		ClassDB::bind_method(D_METHOD("_pause_process", "from"), &TPTHandler::_pause_process);
		ClassDB::bind_method(D_METHOD("_resume_process", "from"), &TPTHandler::_resume_process);
		ClassDB::bind_method(D_METHOD("_end_process", "from"), &TPTHandler::_end_process);
		ClassDB::bind_method(D_METHOD("add_process", "file_name", "block_name", "priority", "run_mode"), &TPTHandler::add_process, DEFVAL(""), DEFVAL(NORMAL), DEFVAL(ADD));
		ClassDB::bind_method(D_METHOD("end"), &TPTHandler::end);
		ClassDB::bind_method(D_METHOD("pause"), &TPTHandler::pause);
		ClassDB::bind_method(D_METHOD("resume"), &TPTHandler::resume);

		BIND_ENUM_CONSTANT(ADD);
		BIND_ENUM_CONSTANT(PAUSE);
		BIND_ENUM_CONSTANT(END);

		BIND_ENUM_CONSTANT(LOW);
		BIND_ENUM_CONSTANT(NORMAL);
		BIND_ENUM_CONSTANT(HIGH);
		BIND_ENUM_CONSTANT(IMMEDIATE);
	}
public:
	static TPTHandler* singleton;
	Ref<TPTProcess> add_process(const String &a_filename, const String &a_blockname = "", const Priority a_priority = NORMAL, const ProcessAddMode a_mode = ADD);
	void end() { call_deferred("_end_process", 0); }
	void pause() { call_deferred("_pause_process", 0); }
	void resume() { call_deferred("_resume_process", 0); }
	TPTHandler() { singleton = this; }
	~TPTHandler() { singleton = nullptr; }
};

VARIANT_ENUM_CAST(TPTHandler::ProcessAddMode);
VARIANT_ENUM_CAST(TPTHandler::Priority);
