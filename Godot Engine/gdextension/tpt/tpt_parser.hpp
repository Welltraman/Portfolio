#pragma once
#include <godot_cpp/classes/file_access.hpp>
#include "tpt_command.hpp"

namespace godot {

class TPTParser {
    static PackedStringArray _load_block(const NodePath& p_tpt_path);
    static PackedStringArray _extract_block(const PackedStringArray& p_contents, const String& p_block);
    static GDCommand* _create_commands(const String& p_cmd_line, Node* p_agent);

    static GDCommand* _create_delay_command(PackedStringArray& p_args, Node* p_agent);
    static GDCommand* _create_next_command(const PackedStringArray& p_args, Node* p_agent);
    static GDCommand* _create_if_command(const PackedStringArray& p_args, Node* p_agent);
    static GDCommand* _create_repeat_begin_command(PackedStringArray& p_args, Node* p_agent);
    static GDCommand* _create_repeat_end_command(PackedStringArray& p_args, Node* p_agent);
    static GDCommand* _create_gdcommand(const String& p_id, const PackedStringArray& p_args);
public:
    static TPTMacro* parse(const NodePath& p_tpt_path, Node* p_agent);
};

}