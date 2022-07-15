#include "config_manager.hpp"

ConfigManager* ConfigManager::singleton = nullptr;

Variant ConfigManager::get_value(const String& a_section, const String& a_key) const {
	return m_config->get_value(a_section, a_key);
}

PoolStringArray ConfigManager::get_section(const String& a_section) const {
	List<String> list = List<String>();
	m_config->get_section_keys(a_section, &list);
	if (list.empty()) return PoolStringArray();
	PoolStringArray array;
	for (List<String>::Element *E = list.front(); E; E = E->next())
		array.append(E->get());
	return array;
}

Dictionary ConfigManager::get_dictionary(const String& a_section) const {
	Dictionary dictionary;
	List<String> sections = List<String>();
	m_config->get_sections(&sections);
	if (sections.empty()) return Dictionary();
	for (List<String>::Element *E = sections.front(); E; E = E->next()) {
		if (a_section != "" && E->get() == a_section) {
			PoolStringArray array = get_section(E->get());
			for (int i = 0; i < array.size(); i++)
				dictionary[array[i]] = m_config->get_value(E->get(), array[i]);
		}
	}
	return dictionary;
}

void ConfigManager::set_config(Ref<ConfigResource> p_config) {
	if (p_config.is_valid())
		m_config = p_config->get_config();
}
