#include "text_file_resource.hpp"

void ResourceFormatLoaderTextFile::get_recognized_extensions(List<String>* p_extensions) const {
	p_extensions->push_back("txt");
}

String ResourceFormatLoaderTextFile::get_resource_type(const String& a_path) const {
	String string = a_path.get_extension().to_lower();
	if (string == "txt")
		return "TextFileResource";
	return "";
}

bool ResourceFormatLoaderTextFile::handles_type(const String& a_type) const {
	return a_type == "TextFileResource";
}

RES ResourceFormatLoaderTextFile::load(const String& a_path, const String& a_original_path, Error* p_error) {
	Ref<TextFileResource> resource = memnew(TextFileResource);
	ERR_FAIL_COND_V_MSG(resource->load_text(a_path), RES(), "Failed to load TextFile resource.");
	resource->set_file_path(a_original_path);
	if (p_error) { *p_error = OK; }
	return resource;
}

void ResourceFormatSaverTextFile::get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const {
	if (Object::cast_to<TextFileResource>(*a_resource)) {
		p_extensions->push_back("txt");
	}
}

bool ResourceFormatSaverTextFile::recognize(const RES& a_resource) const {
	return Object::cast_to<TextFileResource>(*a_resource) != nullptr;
}

Error ResourceFormatSaverTextFile::save(const String& a_path, const RES& a_resource, uint32_t a_flags) {
	Error err;
	FileAccess *file = FileAccess::open(a_path, FileAccess::WRITE, &err);
	ERR_FAIL_COND_V_MSG(err, err, vformat("Could not open '%s' while saving.", a_path));
	Ref<TextFileResource> resource = a_resource;
	file->store_string(resource->get_text());
	file->close();
	memdelete(file);
	return OK;
}
