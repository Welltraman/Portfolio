#pragma once
#include "print_string_extended.hpp"

static LogMsg logmsg = LogMsg();

String LogMsg::_get_date() const {
	OS::Date d = OS::get_singleton()->get_date(false);
	return vformat("%s/%s/%s", d.year, d.month, d.day);
}

String LogMsg::_get_time() const {
	OS::Time t = OS::get_singleton()->get_time(false);
	return vformat("%s:%s:%s", t.hour, t.min, t.sec);
}

String LogMsg::_get_error_text(const Error a_err) const {
	switch (a_err) {
	case OK:
		return "OK";
	case FAILED:
		return "FAILED";
	default:
		return vformat("Error Code: %s", a_err);
	}
}

String LogMsg::compose_message(const String& a_message, const Error a_err, const String& a_logtype) const {
	return vformat("[%s|%s] %s: %s || %s",
		_get_date(),
		_get_time(),
		a_logtype,
		a_message,
		_get_error_text(a_err));
}

void print_debug(const String& a_string, const Error a_err) {
	print_line(logmsg.compose_message(a_string, a_err, "DEBUG_TOOLS"));
}

void print_err(const String& a_string, const Error a_err) {
	ERR_PRINT(logmsg.compose_message(a_string, a_err, "ERROR"));
}

void print_raw(const String& a_string, const Error a_err) {
	OS::get_singleton()->print("%s\n", logmsg.compose_message(a_string, a_err, "INFO").utf8().get_data());
}

void print_thread(const String& a_func_name) {
	std::hash<std::thread::id> hash;
	print_debug(vformat("%s: I'm in thread '%s'.", a_func_name, hash(std::this_thread::get_id())));
}

void print_warn(const String& a_string, const Error a_err) {
	WARN_PRINT(logmsg.compose_message(a_string, a_err, "WARNING"));
}
