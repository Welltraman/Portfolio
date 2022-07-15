#include "tpt_resource.hpp"

List<String> TPTResource::_organize_text(const String &a_text) const {
	Vector<String> vector = a_text.split("\n", false);
	List<String> list;
	for (int i = 0; i < vector.size(); i++)
		list.push_back(vector[i]);
	_organize_text_connect_lines(list);
	return list;
}

void TPTResource::_organize_text_connect_lines(List<String> &r_list) const {
	for (List<String>::Element *E = r_list.front(); E; E = E->next()) {
		E->set(E->get().strip_edges());
		if (E->get() == "")
			r_list.erase(E);
		else if (E->get().ends_with("\\")) {
			E->set(E->get().trim_suffix("\\").strip_edges() + E->next()->get());
			r_list.erase(E->next());
			E = E->prev();
		}
	}
}

Error TPTResource::load(const String& a_path) {
	m_organized_text = _organize_text(get_text());
	return OK;
}

void ResourceFormatLoaderTPT::get_recognized_extensions(List<String>* p_extensions) const {
	p_extensions->push_back("tpt");
}

String ResourceFormatLoaderTPT::get_resource_type(const String& a_path) const {
	String string = a_path.get_extension().to_lower();
	if (string == "tpt")
		return "TPTResource";
	return "";
}

bool ResourceFormatLoaderTPT::handles_type(const String& a_type) const {
	return a_type == "TPTResource";
}

RES ResourceFormatLoaderTPT::load(const String& a_path, const String& a_original_path, Error* p_error) {
	Ref<TPTResource> res = memnew(TPTResource);
	ERR_FAIL_COND_V_MSG(res->load_text(a_path), RES(), "Failed to load TPT text resource.");
	ERR_FAIL_COND_V_MSG(res->load(a_path), RES(), "Failed to load TPT resource.");
	res->set_file_path(a_original_path);
	if (p_error) *p_error = OK;
	return res;
}

void ResourceFormatSaverTPT::get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const {
	if (Object::cast_to<TPTResource>(*a_resource)) {
		p_extensions->push_back("tpt");
	}
}

bool ResourceFormatSaverTPT::recognize(const RES& a_resource) const {
	return Object::cast_to<TPTResource>(*a_resource) != nullptr;
}

Error ResourceFormatSaverTPT::save(const String& a_path, const RES& a_resource, uint32_t a_flags) {
	Error err;
	FileAccess *file = FileAccess::open(a_path, FileAccess::WRITE, &err);
	ERR_FAIL_COND_V_MSG(err, err, "Cannot open TPT file '" + a_path + "'.");
	Ref<TPTResource> res = a_resource;
	file->store_string(res->get_text());
	file->close();
	memdelete(file);
	return OK;
}
