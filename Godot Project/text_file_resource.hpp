#pragma once
#include "core/os/file_access.h"
#include "scene/resources/text_file.h"

class TextFileResource : public TextFile {
	GDCLASS(TextFileResource, TextFile);
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("has_text"), &TextFileResource::has_text);
		ClassDB::bind_method(D_METHOD("get_text"), &TextFileResource::get_text);
		ClassDB::bind_method(D_METHOD("set_text", "text"), &TextFileResource::set_text);
		ClassDB::bind_method(D_METHOD("reload_file"), &TextFileResource::reload_from_file);
	}
};

class ResourceFormatLoaderTextFile : public ResourceFormatLoader {
public:
	void get_recognized_extensions(List<String>* p_extensions) const;
	String get_resource_type(const String& a_path) const;
	bool handles_type(const String& a_type) const;
	RES load(const String& a_path, const String& a_original_path = "", Error* p_error = nullptr);
};

class ResourceFormatSaverTextFile : public ResourceFormatSaver {
public:
	void get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const;
	bool recognize(const RES& a_resource) const;
	Error save(const String& a_path, const RES& a_resource, uint32_t a_flags = 0);
};
