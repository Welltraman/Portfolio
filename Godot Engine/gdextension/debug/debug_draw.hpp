#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include "../convertor.hpp"

namespace godot {

class DebugDraw : public Node2D {
	GDCLASS(DebugDraw, Node2D);

	PackedVector2Array coords = PackedVector2Array();
	_FORCE_INLINE_ PackedVector2Array _get_coords() const { return coords; }
	_FORCE_INLINE_ void _set_coords(const PackedVector2Array& p_coords) { coords = p_coords; }

	bool draw_points = false;
	bool draw_lines = false;
	bool draw_path = false;
	bool draw_polygon = false;
	_FORCE_INLINE_ bool _get_draw_points() const { return draw_points; }
	void _set_draw_points(const bool p_bool);
	_FORCE_INLINE_ bool _get_draw_lines() const { return draw_lines; }
	void _set_draw_lines(const bool p_bool);
	_FORCE_INLINE_ bool _get_draw_path() const { return draw_path; }
	void _set_draw_path(const bool p_bool);
	_FORCE_INLINE_ bool _get_draw_polygon() const { return draw_polygon; }
	void _set_draw_polygon(const bool p_bool);

	MacroCommand* queue = nullptr;

	void _initialize();
	template <typename T>
	void _edit_queue(const bool p_bool) {
		if (p_bool) {
			queue->add(new T(this));
		} else {
			for (const List<Command*>::Element* E = queue->get_list().front(); E; E = E->next()) {
				if (typeid(*(E->get())).name() == typeid(T).name()) {
					queue->remove(E);
				}
			}
		}
	}

protected:
	void _notification(int p_what);
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("_get_coords"), &DebugDraw::_get_coords);
		ClassDB::bind_method(D_METHOD("_set_coords"), &DebugDraw::_set_coords);
		ClassDB::add_property("DebugDraw", PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "coords"), "_set_coords", "_get_coords");
		ClassDB::bind_method(D_METHOD("_get_draw_points"), &DebugDraw::_get_draw_points);
		ClassDB::bind_method(D_METHOD("_set_draw_points"), &DebugDraw::_set_draw_points);
		ClassDB::add_property("DebugDraw", PropertyInfo(Variant::BOOL, "draw points"), "_set_draw_points", "_get_draw_points");
		ClassDB::bind_method(D_METHOD("_get_draw_lines"), &DebugDraw::_get_draw_lines);
		ClassDB::bind_method(D_METHOD("_set_draw_lines"), &DebugDraw::_set_draw_lines);
		ClassDB::add_property("DebugDraw", PropertyInfo(Variant::BOOL, "draw lines"), "_set_draw_lines", "_get_draw_lines");
		ClassDB::bind_method(D_METHOD("_get_draw_path"), &DebugDraw::_get_draw_path);
		ClassDB::bind_method(D_METHOD("_set_draw_path"), &DebugDraw::_set_draw_path);
		ClassDB::add_property("DebugDraw", PropertyInfo(Variant::BOOL, "draw path"), "_set_draw_path", "_get_draw_path");
		ClassDB::bind_method(D_METHOD("_get_draw_polygon"), &DebugDraw::_get_draw_polygon);
		ClassDB::bind_method(D_METHOD("_set_draw_polygon"), &DebugDraw::_set_draw_polygon);
		ClassDB::add_property("DebugDraw", PropertyInfo(Variant::BOOL, "draw polygon"), "_set_draw_polygon", "_get_draw_polygon");
	}
};

class DrawCommand : public Command {
protected:
	DebugDraw* node = nullptr;
public:
	DrawCommand(DebugDraw* p_node): node(p_node) {}
};

class DrawPoints : public DrawCommand {
public:
	_FORCE_INLINE_ StringName get_id() const override { return _STR(DrawPoints); }
	Variant execute() override;
	using DrawCommand::DrawCommand;
};

class DrawLines : public DrawCommand {
public:
	_FORCE_INLINE_ StringName get_id() const override { return _STR(DrawLines); }
	Variant execute() override;
	using DrawCommand::DrawCommand;
};

class DrawPath : public DrawCommand {
public:
	_FORCE_INLINE_ StringName get_id() const override { return _STR(DrawPath); }
	Variant execute() override;
	using DrawCommand::DrawCommand;
};

class DrawPolygon : public DrawCommand {
public:
	_FORCE_INLINE_ StringName get_id() const override { return _STR(DrawPolygon); }
	Variant execute() override;
	using DrawCommand::DrawCommand;
};

}