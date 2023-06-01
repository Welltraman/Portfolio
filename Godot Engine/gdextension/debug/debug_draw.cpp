#include "debug_draw.hpp"

using namespace godot;

Variant DrawPoints::execute() {
	for (int i = 0; i < node->coords.size(); i++) {
		node->draw_circle(node->coords[i], 2, Color());
	}
    return OK;
}

Variant DrawLines::execute() {
    for (int i = 0; i < node->coords.size(); i += 2) {
        if (i == node->coords.size() - 1) {
            break;
        }
        PackedVector2Array conn;
        conn.push_back(node->coords[i]);
        conn.push_back(node->coords[i + 1]);
        node->draw_polyline(conn, Color(1, 0, 0));
    }
    return OK;
}

Variant DrawPath::execute() {
    if (node->coords.size() >= 2) {
        node->draw_polyline(node->coords, Color(1, 0, 0));
    }
    return OK;
}

Variant DrawPolygon::execute() {
    if (node->coords.size() >= 3) {
        node->draw_colored_polygon(node->coords, Color(0, 0, 1));
    }
    return OK;
}

void DebugDraw::_initialize() {
    set_z_index(1000);
    queue = new MacroCommand();
    _set_draw_lines(draw_lines);
    _set_draw_path(draw_path);
    _set_draw_points(draw_points);
    _set_draw_polygon(draw_polygon);
}

void DebugDraw::_notification(int p_what) {
    switch(p_what) {
        case NOTIFICATION_ENTER_TREE: {
            set_process(true);
            _initialize();
        } break;
        case NOTIFICATION_PROCESS: {
            queue_redraw();
        } break;
        case NOTIFICATION_DRAW: {
            queue->execute();
        } break;
    }
}

void DebugDraw::_set_draw_points(const bool p_bool) {
    draw_points = p_bool;
    if (queue) {
        _edit_queue<DrawPoints>(p_bool);
    }
}

void DebugDraw::_set_draw_lines(const bool p_bool) {
    draw_lines = p_bool;
    if (queue) {
        _edit_queue<DrawLines>(p_bool);
    }
}

void DebugDraw::_set_draw_path(const bool p_bool) {
    draw_path = p_bool;
    if (queue) {
        _edit_queue<DrawPath>(p_bool);
    }
}

void DebugDraw::_set_draw_polygon(const bool p_bool) {
    draw_polygon = p_bool;
    if (queue) {
        _edit_queue<DrawPolygon>(p_bool);
    }
}