#pragma once
#include "core/io/json.h"
#include "text_file_resource.hpp"

class JSONResource : public TextFileResource {
	GDCLASS(JSONResource, TextFileResource);
	Variant m_value;
	Variant _parse_json(const String& a_content) const;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("get_value"), &JSONResource::get_value);
	}
public:
	Variant get_value() const { return m_value; }
	Error load(const String& a_path);
};

class ResourceFormatLoaderJSON : public ResourceFormatLoader {
public:
	void get_recognized_extensions(List<String>* p_extensions) const;
	String get_resource_type(const String& a_path) const;
	bool handles_type(const String& a_type) const;
	RES load(const String& a_path, const String& a_original_path = "", Error* p_error = nullptr);
};

class ResourceFormatSaverJSON : public ResourceFormatSaver {
public:
	void get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const;
	bool recognize(const RES& a_resource) const;
	Error save(const String& a_path, const RES& a_resource, uint32_t a_flags = 0);
};
