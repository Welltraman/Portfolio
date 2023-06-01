#pragma once
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include "command.hpp"
#include "factory.hpp"

namespace godot {

const char32_t NODEPATH_SIGN = '?';
const char32_t COMMAND_SIGN = '/';

class Convertor {
    static Variant _convert(const String& p_str);
    static Variant _convert(const PackedStringArray& p_str_arr);

    static Variant _parse_node_path(const NodePath& p_path);
    static String _parse_node_path_command(const String& p_str);
    static Variant _parse_node_path_get_node_or_param(const NodePath& p_path);
    static Variant _parse_command(const String& p_str);
public:
    static Variant convert(const Variant& p_arg);
    static Variant parse(const String& p_str);
};

}