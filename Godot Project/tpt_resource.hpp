#pragma once
#include "text_file_resource.hpp"

class TPTResource : public TextFileResource {
	GDCLASS(TPTResource, TextFileResource);
	List<String> m_organized_text;
	List<String> _organize_text(const String& a_text) const;
	void _organize_text_connect_lines(List<String> &r_list) const;
public:
	List<String> get_texts() { return m_organized_text; }
	Error load(const String& a_path);
};

class ResourceFormatLoaderTPT : public ResourceFormatLoader {
public:
	void get_recognized_extensions(List<String>* p_extensions) const;
	String get_resource_type(const String& a_path) const;
	bool handles_type(const String& a_type) const;
	RES load(const String& a_path, const String& a_original_path = "", Error* p_error = nullptr);
};

class ResourceFormatSaverTPT : public ResourceFormatSaver {
public:
	void get_recognized_extensions(const RES& a_resource, List<String>* p_extensions) const;
	bool recognize(const RES& a_resource) const;
	Error save(const String& a_path, const RES& a_resource, uint32_t a_flags = 0);
};
