#include "command_prompt.hpp"

using namespace godot;

void CommandPrompt::_notification(int p_what) {
    if (p_what == NOTIFICATION_PROCESS && is_visible()) {
        label->set_text(log->get_as_text());
    }
}

void CommandPrompt::_handle_input(const String& p_input) {
    input->clear();
    UtilityFunctions::print(Convertor::parse(p_input));
}

void CommandPrompt::_on_popup() {
    input->grab_focus();
    input->clear();
}

CommandPrompt::CommandPrompt() {
    VBoxContainer* vbc = memnew(VBoxContainer);
    add_child(vbc);

    label = memnew(RichTextLabel);
    label->set_custom_minimum_size(Vector2(230, 265));
    label->set_scroll_follow(true);
    vbc->add_child(label);

    input = memnew(LineEdit);
    input->connect("text_submitted", Callable(this, "_handle"));
    vbc->add_child(input);

    connect("about_to_popup", Callable(this, "_on_popup"));

    log = FileAccess::open("user://logs/godot.log", FileAccess::READ);
}