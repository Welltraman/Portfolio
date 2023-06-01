#pragma once
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include "tpt_command.hpp"

namespace godot {

class TPTMethods : public Object {
    GDCLASS(TPTMethods, Object);

    void _delay(const float p_sec, const Node* p_agent);
    void _next(const String& p_next_block, const Node* p_agent);
    void _if(const Variant& p_cond, const String& p_next_block, const Node* p_agent);
    void _repeat_begin(const int p_count, const Node* p_agent);
    void _repeat_end(const Variant& p_cond, const Node* p_agent, const int p_count = 0);

    void _delay_wait_signal(const float p_sec, Node* p_agent);
    void _delay_wait_timer(const float p_sec, Node* p_agent);

    inline static TPTMethods* singleton = nullptr;

protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD(String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_DELAY).c_str())), &TPTMethods::_delay);
        ClassDB::bind_method(D_METHOD(String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_NEXT).c_str())), &TPTMethods::_next);
        ClassDB::bind_method(D_METHOD(String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_IF).c_str())), &TPTMethods::_if);
        ClassDB::bind_method(D_METHOD(String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_BEGIN).c_str())), &TPTMethods::_repeat_begin);
        ClassDB::bind_method(D_METHOD(String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_END).c_str())), &TPTMethods::_repeat_end, DEFVAL(0));
    }

public:
    static TPTMethods* get_singleton() { return singleton; }
    TPTMethods() { singleton = this; }
    ~TPTMethods() { singleton = nullptr; }
};

}