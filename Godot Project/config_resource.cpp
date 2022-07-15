#include "config_resource.hpp"

Error ConfigResource::load(const String& a_path) {
	m_config = memnew(ConfigFile);
	Vector<String> lines = get_text().split("\n", false);
	String section = "";
	for (int i = 0; i < lines.size(); i++) {
		if (lines[i].begins_with(String::chr(TK_ARRAY_OPEN))) section = lines[i].substr(1, lines[i].length() - 2);
		if (lines[i].begins_with(String::chr(TK_ARRAY_OPEN)) || lines[i].begins_with(";")) continue;
		Vector<String> pair = lines[i].split(String::chr(TK_EQUAL));
		if (pair.size() == 2)
			m_config->set_value(section, pair[0], convert_string(pair[1]));
	}
	return OK;
}

void ResourceFormatLoaderConfig::get_recognized_extensions(List<String>* p_extensions) const {
	p_extensions->push_back("cfg");
	p_extensions->push_back("ini");
}

String ResourceFormatLoaderConfig::get_resource_type(const String& a_path) const {
	String string = a_path.get_extension().to_lower();
	if (string == "cfg" || string == "ini")
		return "ConfigResource";
	return "";
}

bool ResourceFormatLoaderConfig::handles_type(const String& a_type) const {
	return a_type == "ConfigResource";
}

RES ResourceFormatLoaderConfig::load(const String& a_path, const String& a_original_path, Error* p_error) {
	Ref<ConfigResource> res = memnew(ConfigResource);
	ERR_FAIL_COND_V_MSG(res->load_text(a_path), RES(), "Failed to load Config text resource.");
	ERR_FAIL_COND_V_MSG(res->load(a_path), RES(), "Failed to load Config resource.");
	res->set_file_path(a_original_path);
	if (p_error) *p_error = OK;
	return res;
}

void ResourceFormatSaverConfig::get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const {
	if (Object::cast_to<ConfigResource>(*a_resource)) {
		p_extensions->push_back("cfg");
		p_extensions->push_back("ini");
	}
}

bool ResourceFormatSaverConfig::recognize(const RES& a_resource) const {
	return Object::cast_to<ConfigResource>(*a_resource) != nullptr;
}

Error ResourceFormatSaverConfig::save(const String& a_path, const RES& a_resource, uint32_t a_flags) {
	Error err;
	FileAccess* file = FileAccess::open(a_path, FileAccess::WRITE, &err);
	ERR_FAIL_COND_V_MSG(err, err, "Cannot open Config file '" + a_path + "'.");
	Ref<ConfigResource> res = a_resource;
	file->store_string(res->get_text());
	file->close();
	memdelete(file);
	return OK;
}
