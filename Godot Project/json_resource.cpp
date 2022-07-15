#include "json_resource.hpp"

Variant JSONResource::_parse_json(const String& a_content) const {
	Variant ret;
	String err_msg;
	int line;
	ERR_FAIL_COND_V_MSG(JSON().parse(a_content, ret, err_msg, line), NULL, err_msg + vformat(" at line: %s", line));
	return ret;
}

Error JSONResource::load(const String& a_path) {
	m_value = _parse_json(get_text());
	return OK;
}

void ResourceFormatLoaderJSON::get_recognized_extensions(List<String>* p_extensions) const {
	p_extensions->push_back("json");
}

String ResourceFormatLoaderJSON::get_resource_type(const String& a_path) const {
	String string = a_path.get_extension().to_lower();
	if (string == "json")
		return "JSONResource";
	return "";
}

bool ResourceFormatLoaderJSON::handles_type(const String& a_type) const {
	return a_type == "JSONResource";
}

RES ResourceFormatLoaderJSON::load(const String& a_path, const String& a_original_path, Error* p_error) {
	Ref<JSONResource> res = memnew(JSONResource);
	ERR_FAIL_COND_V_MSG(res->load_text(a_path), RES(), "Failed to load JSON text resource.");
	ERR_FAIL_COND_V_MSG(res->load(a_path), RES(), "Failed to load JSON resource.");
	res->set_file_path(a_original_path);
	if (p_error) *p_error = OK;
	return res;
}

void ResourceFormatSaverJSON::get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const {
	if (Object::cast_to<JSONResource>(*a_resource)) {
		p_extensions->push_back("json");
	}
}

bool ResourceFormatSaverJSON::recognize(const RES& a_resource) const {
	return Object::cast_to<JSONResource>(*a_resource) != nullptr;
}

Error ResourceFormatSaverJSON::save(const String& a_path, const RES& a_resource, uint32_t a_flags) {
	Error err;
	FileAccess* file = FileAccess::open(a_path, FileAccess::WRITE, &err);
	ERR_FAIL_COND_V_MSG(err, err, "Cannot open JSON file '" + a_path + "'.");
	Ref<JSONResource> res = a_resource;
	file->store_string(JSON().print(res->get_value(), "\t"));
	file->close();
	memdelete(file);
	return OK;
}
