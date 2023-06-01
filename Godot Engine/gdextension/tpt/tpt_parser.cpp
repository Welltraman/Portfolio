#include "tpt_parser.hpp"

using namespace godot;

TPTMacro* TPTParser::parse(const NodePath& p_tpt_path, Node* p_agent) {
    PackedStringArray block = _load_block(p_tpt_path);
    ERR_FAIL_COND_V_MSG(block.is_empty(), NULL, "TPT file 'res://" + p_tpt_path + "' does not have the block.");
    TPTMacro* macro = new TPTMacro;
    for (int i = 0; i < block.size(); i++) {
        GDCommand* cmd = _create_commands(block[i], p_agent);
        if (cmd) {
            macro->add(cmd);
        }
    }
    return macro;
}

PackedStringArray TPTParser::_load_block(const NodePath& p_tpt_path) {
    Ref<FileAccess> file = FileAccess::open("res://" + p_tpt_path.get_concatenated_names(), FileAccess::READ);
    ERR_FAIL_COND_V_MSG(file.is_null(), PackedStringArray(), "TPT file 'res://" + p_tpt_path.get_concatenated_names() + "' could not be loaded.");
    return _extract_block(file->get_as_text().split("\n", false), ":" + p_tpt_path.get_concatenated_subnames());
}

PackedStringArray TPTParser::_extract_block(const PackedStringArray& p_contents, const String& p_block) {
    PackedStringArray ret;
    int beginning = (p_block == "") ? 0 : p_contents.find(p_block) + 1;
    for (int i = beginning; i < p_contents.size(); i++) {
        if (TPTTokenizer::get_token(p_contents[i]) == TPTTokenizer::TK_BLOCK) {
            break;
        }
        ret.push_back(p_contents[i]);
    }
    return ret;
}

GDCommand* TPTParser::_create_commands(const String& p_cmd_line, Node* p_agent) {
    PackedStringArray args = p_cmd_line.substr(1, p_cmd_line.length() - 1).split(" ", false);
    switch (TPTTokenizer::get_token(p_cmd_line)) {
        case TPTTokenizer::TK_COMMAND: {
            String id = args[0];
            args.remove_at(0);
            return _create_gdcommand(id, args);
        } break;
        case TPTTokenizer::TK_DELAY: {
            return _create_delay_command(args, p_agent);
        } break;
        case TPTTokenizer::TK_IF: {
            return _create_if_command(args, p_agent);
        } break;
        case TPTTokenizer::TK_NEXT: {
            return _create_next_command(p_cmd_line.substr(2, p_cmd_line.length() - 2).split(" ", false), p_agent);
        } break;
        case TPTTokenizer::TK_REPEAT_BEGIN: {
            return _create_repeat_begin_command(args, p_agent);
        } break;
        case TPTTokenizer::TK_REPEAT_END: {
            return _create_repeat_end_command(args, p_agent);
        } break;
        default: {
            ERR_FAIL_V_MSG(nullptr, "TPTParser could not parse this line: " + p_cmd_line);
        }
    }
}

GDCommand* TPTParser::_create_gdcommand(const String& p_id, const PackedStringArray& p_args) {
    return Factory<GDCommand, Array>::get(p_id.utf8().get_data(), Convertor::convert(p_args));
}

GDCommand* TPTParser::_create_delay_command(PackedStringArray& p_args, Node* p_agent) {
    if (p_args.is_empty()) {
        p_args.push_back("0");
    } else if (!p_args[0].contains(".")) {
        p_args[0] += ".0";
    }
    Array args = Convertor::convert(p_args);
    args.push_back(p_agent);
    return Factory<GDCommand, Array>::get(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_DELAY), args);
}

GDCommand* TPTParser::_create_if_command(const PackedStringArray& p_args, Node* p_agent) {
    Array args = Convertor::convert(p_args);
    args.push_back(p_agent);
    return Factory<GDCommand, Array>::get(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_IF), args);
}

GDCommand* TPTParser::_create_next_command(const PackedStringArray& p_args, Node* p_agent) {
    Array args = Convertor::convert(p_args);
    args.push_back(p_agent);
    return Factory<GDCommand, Array>::get(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_NEXT), args);
}

GDCommand* TPTParser::_create_repeat_begin_command(PackedStringArray& p_args, Node* p_agent) {
    if (p_args.is_empty()) {
        p_args.push_back("0");
    }
    Array args = Convertor::convert(p_args);
    args.push_back(p_agent);
    return Factory<GDCommand, Array>::get(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_BEGIN), args);
}

GDCommand* TPTParser::_create_repeat_end_command(PackedStringArray& p_args, Node* p_agent) {
    if (p_args.is_empty()) {
        p_args.push_back("true");
    }
    Array args = Convertor::convert(p_args);
    args.push_back(p_agent);
    return Factory<GDCommand, Array>::get(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_END), args);
}