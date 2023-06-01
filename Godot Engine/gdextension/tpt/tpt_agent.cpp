#include "tpt_agent.hpp"

using namespace godot;

void TPTAgent::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            if (!tpt_path.is_empty() && !Engine::get_singleton()->is_editor_hint()) {
                _parse(tpt_path);
            }
        } break;
        case NOTIFICATION_READY: {
            if (get_child_count() == 0) {
                start();
            } else {
                _initialize();
            }
        } break;
    }
}

void TPTAgent::_initialize() {
    for (int i = 0; i < get_child_count(); i++) {
        if (get_child(i)->get_class() == _STR(TPTAgent)) {
            ((TPTAgent*)get_child(i))->connect("tpt_done", Callable(this, "start"));
        } else if (get_child(i)->get_class() == _STR(Area2D)) {
            ((Area2D*)get_child(i))->connect("area_entered", Callable(this, "start"));
        }
    }
}

void TPTAgent::start(const String& p_id) {
    if (starter && p_id != starter->get_id()) return;
    SceneTree* tree = (SceneTree*)Engine::get_singleton()->get_main_loop();
    if (enabled && !tree->is_connected("process_frame", Callable(this, "trigger"))) {
        tree->connect("process_frame", Callable(this, "trigger"));
        if (starter) {
            ObjectDB::get_instance(starter->get_owner_id())->disconnect("tpt_done", Callable(this, "start"));
            starter = nullptr;
        }
    }
}

void TPTAgent::stop() {
    SceneTree* tree = (SceneTree*)Engine::get_singleton()->get_main_loop();
    if (tree->is_connected("process_frame", Callable(this, "trigger"))) {
        tree->disconnect("process_frame", Callable(this, "trigger"));
    }
}

void TPTAgent::_parse(const String& p_path) {
    PackedStringArray arr = p_path.split(":", false);
    String path = p_path;
    if (arr.size() == 1 && !arr[0].ends_with(".tpt")) {
        path = arr[0] + ".tpt";
    } else if (arr.size() == 2) {
        path = ((arr[0].ends_with(".tpt")) ? arr[0] : arr[0] + ".tpt") + ":" + arr[1];
    }
    if (queue) delete queue;
    queue = TPTParser::parse(path, this);
}

void TPTAgent::trigger() {
    if (queue && enabled) {
        queue->execute();
        if (!queue->get_current()) {
            emit_signal("tpt_done");
            stop();
        }
    }
}

void TPTAgent::add_new_block(const String& p_next_block) {
    String block = p_next_block;
    if (p_next_block.begins_with(":")) {
        block = NodePath(tpt_path).get_concatenated_names() + block;
    }
    _parse(block);
    start();
}