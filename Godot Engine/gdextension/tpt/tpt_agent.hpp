#pragma once
#include <godot_cpp/classes/area2d.hpp>
#include "tpt_parser.hpp"

namespace godot {

class TPTAgent : public Node {
    GDCLASS(TPTAgent, Node);

    bool enabled = true;
    bool _get_enabled() const { return enabled; }
    void _set_enabled(const bool p_enabled) { enabled = p_enabled; }

    String tpt_path = "";
    String _get_tpt_path() const { return tpt_path; }
    void _set_tpt_path(const String& p_path) { tpt_path = p_path; }

    TPTMacro* queue = nullptr;

    void _initialize();
    void _parse(const String& p_path);

protected:
    void _notification(int p_what);
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_get_enabled"), &TPTAgent::_get_enabled);
        ClassDB::bind_method(D_METHOD("_set_enabled"), &TPTAgent::_set_enabled);
        ClassDB::add_property("TPTAgent", PropertyInfo(Variant::BOOL, "enabled"), "_set_enabled", "_get_enabled");
        ClassDB::bind_method(D_METHOD("_get_tpt_path"), &TPTAgent::_get_tpt_path);
        ClassDB::bind_method(D_METHOD("_set_tpt_path"), &TPTAgent::_set_tpt_path);
        ClassDB::add_property("TPTAgent", PropertyInfo(Variant::STRING, "tpt path"), "_set_tpt_path", "_get_tpt_path");
        ClassDB::bind_method(D_METHOD("start", "id"), &TPTAgent::start, DEFVAL(""));
        ClassDB::bind_method(D_METHOD("stop"), &TPTAgent::stop);
        ClassDB::bind_method(D_METHOD("trigger"), &TPTAgent::trigger);
        ClassDB::bind_method(D_METHOD("add_new_block"), &TPTAgent::add_new_block);

        ClassDB::add_signal("TPTAgent", MethodInfo("tpt_done"));
    }

public:
    GDCommand* starter = nullptr;

    void start(const String& p_id = "");
    void stop();
    TPTMacro* get_queue() const { return queue; }
    void add_new_block(const String& p_next_block);
    void trigger();
};

}