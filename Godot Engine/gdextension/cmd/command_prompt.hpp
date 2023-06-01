#pragma once
#include <godot_cpp/classes/popup_panel.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include "../convertor.hpp"

namespace godot {

class CommandPrompt : public PopupPanel {
    GDCLASS(CommandPrompt, PopupPanel);

    RichTextLabel* label = nullptr;
    LineEdit* input = nullptr;
    Ref<FileAccess> log = nullptr;
    _FORCE_INLINE_ Ref<Theme> _get_theme() const { return get_theme(); }
    _FORCE_INLINE_ void _set_theme(const Ref<Theme> p_theme) { set_theme(p_theme); }

    void _handle_input(const String& p_input);
    void _on_popup();

protected:
    void _notification(int p_what);
	static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_handle", "input"), &CommandPrompt::_handle_input);
        ClassDB::bind_method(D_METHOD("_on_popup"), &CommandPrompt::_on_popup);
        ClassDB::bind_method(D_METHOD("_set_theme"), &CommandPrompt::_set_theme);
        ClassDB::bind_method(D_METHOD("_get_theme"), &CommandPrompt::_get_theme);
        ClassDB::add_property("CommandPrompt", PropertyInfo(Variant::OBJECT, "theme", PROPERTY_HINT_RESOURCE_TYPE, "", PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT), "_set_theme", "_get_theme");
    }

public:
    CommandPrompt();
};

}
