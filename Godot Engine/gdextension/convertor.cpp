#include "convertor.hpp"

using namespace godot;

Variant Convertor::convert(const Variant& p_arg) {
    switch (p_arg.get_type()) {
        case Variant::STRING: {
            return _convert((String)p_arg);
        } break;
        case Variant::PACKED_STRING_ARRAY: {
            return _convert((PackedStringArray)p_arg);
        } break;
        default: {
            return UtilityFunctions::var_to_str(p_arg);
        }
    }
}

Variant Convertor::_convert(const String& p_str) {
    Variant v = UtilityFunctions::str_to_var(p_str);
    if (v.get_type() == Variant::NIL) {
        return p_str;
    }
    return v;
}

Variant Convertor::_convert(const PackedStringArray& p_str_arr) {
    Array arr;
    for (int i = 0; i < p_str_arr.size(); i++) {
        arr.push_back(_convert(p_str_arr[i]));
    }
    return arr;
}

Variant Convertor::parse(const String& p_str) {
    switch(p_str[0]) {
        case NODEPATH_SIGN: {
            return _parse_node_path(p_str.substr(1, p_str.length() - 1).strip_escapes());
        } break;
        case COMMAND_SIGN: {
            return _parse_command(p_str.substr(1, p_str.length() - 1).strip_escapes());
        } break;
    }
    return p_str;
}

Variant Convertor::_parse_node_path(const NodePath& p_path) {
    if (p_path.is_empty()) return Variant();
    NodePath path = p_path;
    if (!path.is_absolute()) {
        String leftover = ((String)p_path).replace(p_path.get_name(0), "");
        path = _parse_node_path_command(p_path.get_name(0)) + leftover;
    }
    return _parse_node_path_get_node_or_param(path);
}

String Convertor::_parse_node_path_command(const String& p_str) {
    Variant target = _parse_command(p_str);
    switch (target.get_type()) {
        case Variant::STRING: {
            return (target.operator String().begins_with("/")) ? target : "/root/" + (String)target;
        } break;
        case Variant::OBJECT: {
            Node* node = (Node*)(Object*)target;
            return node->get_path();
        } break;
        default: {
            return "/root/" + p_str;
        }
    }
}

Variant Convertor::_parse_node_path_get_node_or_param(const NodePath& p_path) {
    SceneTree* tree = (SceneTree*)Engine::get_singleton()->get_main_loop();
    Object* node = tree->get_root()->get_node<Object>(p_path);
    if (p_path.get_subname_count() > 0 && node) {
        return node->get_indexed(NodePath(p_path.get_concatenated_subnames()));
    } else {
        return node;
    }
}

Variant Convertor::_parse_command(const String& p_str) {
    PackedStringArray str_arr = p_str.split(" ", false);
    String id = str_arr[0];
    str_arr.remove_at(0);
    GDCommand* cmd = Factory<GDCommand, Array>::get(id.utf8().get_data(), convert(str_arr));
    if (cmd) {
        return cmd->execute();
    } else {
        return Variant();
    }
}