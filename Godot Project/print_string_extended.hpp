#pragma once
#include "core/os/os.h"
#include <thread>

class LogMsg {
	String _get_date() const;
	String _get_time() const;
	String _get_error_text(const Error a_err) const;
public:
	String compose_message(const String& a_message, const Error a_err, const String& a_logtype) const;
};

void print_debug(const String& a_string, const Error a_err = OK);
void print_err(const String& a_string, const Error a_err = FAILED);
void print_raw(const String& a_string, const Error a_err = OK);
void print_thread(const String& a_func_name);
void print_warn(const String& a_string, const Error a_err = FAILED);
