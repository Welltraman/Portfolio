#include "tpt_handler.hpp"

List<Ref<TPTMethodBase>>* TPTParser::parse(const String& a_filename, const String& a_blockname) {
	ParseInfo::Element *info = m_parsed_tpts.find(a_filename);
	if (!info)
		info = _parse(a_filename);
	if (info->get().has(a_blockname))
		return &info->get()[a_blockname];
	else {
		print_warn(vformat("'%s' does not have block called '%s'.", a_filename, a_blockname));
		return nullptr;
	}
}

TPTParser::ParseInfo::Element *TPTParser::_parse(const String &a_filename) {
	ParseInfo::Element *info = m_parsed_tpts.push_back(FileParseInfo(a_filename));
	ResourceManager *rm = (ResourceManager *)Engine::get_singleton()->get_singleton_object("RM");
	Ref<TPTResource> resource = rm->get_asset(a_filename + ".tpt");
	List<String> list = resource->get_texts();
	for (List<String>::Element* E = list.front(); E; E = E->next())
		_curate_tpt(E->get()[0], E->get().substr(1).strip_edges(), info->get());
	_debug_print_blocks(a_filename);
	return info;
}

void TPTParser::_curate_tpt(const CharType a_type, const String &a_text, FileParseInfo &r_info) {
	switch (a_type) {
		case SYM_BLOCK:
			_curate_tpt_block(a_text, r_info);
			break;
		case SYM_COMMENT:
			break;
		case SYM_DELAY:
			_curate_tpt_method(a_text, true, r_info);
			break;
		case SYM_IMPORT:
			_curate_tpt_import(a_text, r_info);
			break;
		case SYM_METHOD:
			_curate_tpt_method(a_text, false, r_info);
			break;
		case SYM_TARGET:
			_curate_tpt_target(a_text, r_info);
			break;
		case SYM_VARIABLE:
			_curate_tpt_variable(a_text, r_info);
			break;
		default:
			print_warn(vformat("'%s' is not a supported symbol.", a_type));
	}
}

void TPTParser::_curate_tpt_block(const String &a_text, FileParseInfo &r_info) {
	if (!r_info.has(a_text))
		r_info.add(a_text);
	else
		r_info.blocks.move_to_back(r_info.blocks.find(a_text));
}

void TPTParser::_curate_tpt_import(const String& a_text, FileParseInfo& r_info) {
	Vector<String> vector = split_outside_quote(a_text, String::chr(TK_COMMA));
	for (int i = 0; i < vector.size(); i++) {
		if (r_info.imported_files.find(vector[i])) continue;
		ParseInfo::Element *info = m_parsed_tpts.find(a_text);
		if (!info) info = _parse(a_text);
		r_info.imported_files.push_back(vector[i]);
		r_info += info->get();
	}
}

void TPTParser::_curate_tpt_method(const String &a_text, const bool a_is_delay, FileParseInfo &r_info) {
	String string = _curate_tpt_method_apply_variables(a_text, r_info);
	Vector<String> vector = split_outside_quote(string, String::chr(TK_EQUAL));
	Ref<TPTMethodBase> method = TPTMethodFactory::get(_curate_tpt_method_get_method_name(vector[0], a_is_delay));
	r_info.blocks.back()->get().methods.push_back(method);
	method->delay = a_is_delay;
	method->targets = r_info.blocks.back()->get().current_targets;
	method->values = _curate_tpt_method_get_values(vector, r_info);
	r_info.set_defaults(method->values);
}

String TPTParser::_curate_tpt_method_apply_variables(const String& a_text, FileParseInfo& r_info) const {
	String string = a_text;
	Map<String, String> map = merge_maps<String, String>(r_info.blocks.back()->get().current_variables, r_info.global_variables);
	for (Map<String, String>::Element* E = map.front(); E; E = E->next())
		string = replace_outside_quote(a_text, E->key(), E->value());
	return string;
}

String TPTParser::_curate_tpt_method_get_method_name(const String& a_text, const bool a_delay) const {
	String string = a_text;
	if (string.is_valid_integer() || string.is_valid_float())
		if (a_delay) string = "wait";
	return string;
}

Array TPTParser::_curate_tpt_method_get_values(const Vector<String> a_raw_values, FileParseInfo& r_info) const {
	Array array;
	if (a_raw_values.size() == 2) array = _curate_tpt_method_convert_values(a_raw_values[1], r_info.get_defaults());
	else if (a_raw_values.size() == 1) {
		if (a_raw_values[0].is_valid_integer() || a_raw_values[0].is_valid_float())
			array.append(a_raw_values[0].to_float());
		else array = r_info.get_defaults();
	}
	return array;
}

Array TPTParser::_curate_tpt_method_convert_values(const String& a_text, const Array& a_arr) const {
	Vector<String> vector = split_outside_quote_and_brackets(a_text);
	Array array;
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i] == String::chr(SYM_REPLACE)) array.append(a_arr[i]);
		else array.append(convert_string(vector[i]));
	}
	if (array.size() < a_arr.size())
		for (int i = array.size(); i < a_arr.size(); i++) array.push_back(a_arr[i]);
	else if (array.size() > a_arr.size()) ERR_FAIL_COND_V(array.resize(a_arr.size()), Array());
	return array;
}

void TPTParser::_curate_tpt_target(const String &a_text, FileParseInfo &r_info) {
	r_info.blocks.back()->get().current_targets = split_outside_quote(a_text, String::chr(TK_COMMA));
}

void TPTParser::_curate_tpt_variable(const String &a_text, FileParseInfo &r_info) {
	Vector<String> vector = split_outside_quote(a_text, String::chr(TK_EQUAL));
	if (vector.size() != 2) return;
	if (r_info.blocks.back()->get().block_name == "")
		r_info.global_variables[vector[0]] = vector[1];
	else
		r_info.blocks.back()->get().current_variables[vector[0]] = vector[1];
}

void TPTParser::_debug_print_blocks(const String &a_filename) {
	print_debug("******** PRINTING BLOCKS ********");
	print_debug(vformat("File name: '%s'", a_filename));
	ParseInfo::Element *file = m_parsed_tpts.find(a_filename);
	for (List<FileParseInfo::BlockParseInfo>::Element *E = file->get().blocks.front(); E; E = E->next()) {
		print_debug(vformat("Block name: '%s'", E->get().block_name));
		for (int i = 0; i < E->get().methods.size(); i++) {
			Array temp;
			for (int j = 0; j < E->get().methods[i]->targets.size(); j++) temp.append(E->get().methods[i]->targets[j]);
			print_debug(vformat("Method name: '%s', delay: '%s', targets: '%s', values: '%s'",
				typeid(**E->get().methods[i]).name(),
				E->get().methods[i]->delay, temp,
				E->get().methods[i]->values));
		}
		print_debug("---------------------------------");
	}
	print_debug("*********************************");
}

//=================================================================================================

Array TPTParser::FileParseInfo::get_defaults() const {
	Ref<TPTMethodBase> current = blocks.back()->get().methods.back()->get();
	String name = typeid(**current).name();
	if (!blocks.back()->get().current_defaults.empty() && blocks.back()->get().current_defaults.has(name))
		return blocks.back()->get().current_defaults[name];
	else if (!global_defaults.empty() && global_defaults.has(name)) return global_defaults[name];
	else return current->get_default_value();
}

void TPTParser::FileParseInfo::set_defaults(const Array& a_value) {
	Ref<TPTMethodBase> &current = blocks.back()->get().methods.back()->get();
	String name = typeid(**current).name();
	if (blocks.back()->get().block_name == "")
		global_defaults.insert(name, a_value);
	else
		blocks.back()->get().current_defaults.insert(name, a_value);
}

void TPTParser::FileParseInfo::operator+=(const FileParseInfo& a_info) {
	global_variables = merge_maps<String, String>(a_info.global_variables, global_variables);
	global_defaults = merge_maps<String, Array>(a_info.global_defaults, global_defaults);
	for (const List<String>::Element *E = a_info.imported_files.front(); E; E = E->next())
		if (imported_files.find(E->get())) imported_files.push_back(E->get());
	for (const List<BlockParseInfo>::Element *E = a_info.blocks.front(); E; E = E->next()) {
		if (!has(E->get().block_name)) blocks.push_back(E->get());
		else blocks.find(E->get().block_name)->get() += E->get();
	}
}

void TPTParser::FileParseInfo::BlockParseInfo::operator+=(const BlockParseInfo& a_info) {
	current_targets = a_info.current_targets;
	current_variables = merge_maps<String, String>(a_info.current_variables, current_variables);
	current_defaults = merge_maps<String, Array>(a_info.current_defaults, current_defaults);
	for (const List<Ref<TPTMethodBase>>::Element *E = a_info.methods.front(); E; E = E->next())
		methods.push_back(E->get());
}
