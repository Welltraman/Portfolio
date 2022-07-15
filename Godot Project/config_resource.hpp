#pragma once
#include "core/io/config_file.h"
#include "string_extended.hpp"
#include "text_file_resource.hpp"

class ConfigResource : public TextFileResource {
	GDCLASS(ConfigResource, TextFileResource);
	ConfigFile* m_config;
protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("get_config"), &ConfigResource::get_config);
	}
public:
	Ref<ConfigFile> get_config() { return m_config; }
	Error load(const String& a_path);
};

class ResourceFormatLoaderConfig : public ResourceFormatLoader {
public:
	void get_recognized_extensions(List<String>* p_extensions) const;
	String get_resource_type(const String& a_path) const;
	bool handles_type(const String& a_type) const;
	RES load(const String& a_path, const String& a_original_path = "", Error* p_error = nullptr);
};

class ResourceFormatSaverConfig : public ResourceFormatSaver {
public:
	void get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const;
	bool recognize(const RES& a_resource) const;
	Error save(const String& a_path, const RES& a_resource, uint32_t a_flags = 0);
};
