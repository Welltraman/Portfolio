#pragma once
#include "config_resource.hpp"

class ConfigManager : public Object {
	GDCLASS(ConfigManager, Object);
	Ref<ConfigFile> m_config;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("get_value", "section", "key"), &ConfigManager::get_value);
		ClassDB::bind_method(D_METHOD("get_section", "section"), &ConfigManager::get_section);
		ClassDB::bind_method(D_METHOD("get_dictionary", "section"), &ConfigManager::get_dictionary, DEFVAL(""));
		ClassDB::bind_method(D_METHOD("set_config", "config_resource"), &ConfigManager::set_config);
	}

public:
	static ConfigManager *singleton;
	Variant get_value(const String &a_section, const String &a_key) const;
	PoolStringArray get_section(const String &a_section) const;
	Dictionary get_dictionary(const String &a_section = "") const;
	void set_config(Ref<ConfigResource> p_config);
	ConfigManager() { singleton = this; }
	~ConfigManager() { singleton = nullptr; }
};
