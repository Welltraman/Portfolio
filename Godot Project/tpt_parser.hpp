#pragma once
#include "string_extended.hpp"
#include "tpt_resource.hpp"
#include "tpt_methods/tptm_base.hpp"

class TPTParser {
	enum {
		SYM_BLOCK = '!',
		SYM_COMMENT = '#',
		SYM_DELAY = '~',
		SYM_IMPORT = '@',
		SYM_METHOD = '-',
		SYM_TARGET = '$',
		SYM_VARIABLE = '+',
		SYM_REPLACE = '_',
	};
	struct FileParseInfo {
		String file_name;
		Map<String, String> global_variables = Map<String, String>();
		Map<String, Array> global_defaults = Map<String, Array>();
		List<String> imported_files;
		struct BlockParseInfo {
			String block_name;
			Vector<String> current_targets;
			Map<String, String> current_variables = Map<String, String>();
			Map<String, Array> current_defaults = Map<String, Array>();
			List<Ref<TPTMethodBase>> methods;
			bool operator==(const String& a_text) { return a_text == block_name; }
			void operator+=(const BlockParseInfo &a_info);
			BlockParseInfo(const String &a_bname = "") : block_name(a_bname) {}
		};
		List<BlockParseInfo> blocks;
		void add(const String &a_blockname) { blocks.push_back(BlockParseInfo(a_blockname)); }
		bool has(const String &a_blockname) { return blocks.find(a_blockname) != nullptr; }
		Array get_defaults() const;
		void set_defaults(const Array &a_value);
		bool operator==(const String &a_text) { return a_text == file_name; }
		List<Ref<TPTMethodBase>>& operator[](const String& a_blockname) { return blocks.find(a_blockname)->get().methods; }
		void operator+=(const FileParseInfo &a_info);
		FileParseInfo(const String &a_fname = "") : file_name(a_fname) { add(""); }
	};
	typedef List<FileParseInfo> ParseInfo;
	ParseInfo m_parsed_tpts;
	void _curate_tpt(const CharType a_type, const String& a_text, FileParseInfo& r_info);
	void _curate_tpt_block(const String &a_text, FileParseInfo &r_info);
	void _curate_tpt_import(const String& a_text, FileParseInfo& r_info);
	void _curate_tpt_method(const String& a_text, const bool a_is_delay, FileParseInfo& r_info);
	String _curate_tpt_method_apply_variables(const String& a_text, FileParseInfo& r_info) const;
	Array _curate_tpt_method_convert_values(const String& a_text, const Array& a_arr) const;
	String _curate_tpt_method_get_method_name(const String& a_text, const bool a_delay) const;
	Array _curate_tpt_method_get_values(const Vector<String> a_raw_values, FileParseInfo& r_info) const;
	void _curate_tpt_target(const String& a_text, FileParseInfo& r_info);
	void _curate_tpt_variable(const String& a_text, FileParseInfo& r_info);
	void _debug_print_blocks(const String& a_filename);
	ParseInfo::Element *_parse(const String &a_filename);
public:
	List<Ref<TPTMethodBase>>* parse(const String& a_filename, const String& a_blockname);
};

template <class K, class V>
Map<K, V> merge_maps(const Map<K, V>& a_1, const Map<K, V>& a_2) {
	if (a_1.empty()) return a_2;
	Map<K, V> temp = a_1;
	if (!a_2.empty()) {
		for (Map<K, V>::Element *E = a_2.front(); E; E = E->next())
			temp[E->key()] = a_2[E->key()];
	}
	return temp;
}
