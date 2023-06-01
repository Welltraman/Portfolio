#include "tpt_method.hpp"
#include "tpt_agent.hpp"

using namespace godot;

void TPTMethods::_delay(const float p_sec, const Node* p_agent) {
    TPTAgent* agent = (TPTAgent*)p_agent;
    if (!agent->get_queue()) return;
    if (p_sec <= 0) {
        _delay_wait_signal(p_sec, agent);
    } else {
        _delay_wait_timer(p_sec, agent);
    }
    agent->stop();
}

void TPTMethods::_delay_wait_signal(const float p_sec, Node* p_agent) {
    List<Command*>::Element* item = ((TPTAgent*)p_agent)->get_queue()->get_current()->prev();
    for (int i = 0; i > p_sec; i--) { item = item->prev(); }
    GDCommand* cmd = (GDCommand*)item->get();
    ((TPTAgent*)p_agent)->starter = cmd;
    Object* caller = ObjectDB::get_instance(cmd->get_owner_id());
    if (!caller->is_connected("tpt_done", Callable(p_agent, "start"))) {
        caller->connect("tpt_done", Callable(p_agent, "start"));
    }
}

void TPTMethods::_delay_wait_timer(const float p_sec, Node* p_agent) {
    SceneTree* tree = (SceneTree*)Engine::get_singleton()->get_main_loop();
    tree->create_timer(p_sec)->connect("timeout", Callable(p_agent, "start"), CONNECT_ONE_SHOT);
}

void TPTMethods::_next(const String& p_next_block, const Node* p_agent) {
    TPTAgent* agent = (TPTAgent*)p_agent;
    String block = p_next_block;
    if (!block.contains(":")) {
        block = ":" + block;
    }
    agent->call_deferred("add_new_block", block);
}

void TPTMethods::_if(const Variant& p_cond, const String& p_next_block, const Node* p_agent) {
    bool b = (p_cond.get_type() == Variant::STRING) ? Convertor::parse("?" + (String)p_cond) : p_cond;
    if (b) {
        TPTAgent* agent = (TPTAgent*)p_agent;
        agent->stop();
        _next(p_next_block, p_agent);
    }
}

void TPTMethods::_repeat_begin(const int p_count, const Node* p_agent) {
    TPTAgent* agent = (TPTAgent*)p_agent;
    GDCommand* cmd = (GDCommand*)agent->get_queue()->find(String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_END).c_str()));
    cmd->args.push_back(p_count - 1);
}

void TPTMethods::_repeat_end(const Variant& p_cond, const Node* p_agent, const int p_count) {
    TPTAgent* agent = (TPTAgent*)p_agent;
    bool b = (p_cond.get_type() == Variant::STRING) ? Convertor::parse(p_cond) : p_cond;
    if (b && p_count != 0) {
        // duplicate repeat end command
        GDCommand* dupe = (GDCommand*)agent->get_queue()->get_current()->get();
        dupe->args[dupe->args.size() - 1] = p_count - 1;
        agent->get_queue()->add(dupe, agent->get_queue()->get_current());
        // add other commands in between
        List<Command*>::Element* previous = agent->get_queue()->get_current()->prev();
        for (previous; previous != agent->get_queue()->get_list().front(); previous = previous->prev()) {
            if (previous->get()->get_id() == String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_END).c_str())
            || previous->get()->get_id() == String(TPTTokenizer::get_tpt_method_id(TPTTokenizer::TK_REPEAT_BEGIN).c_str())) {
                break;
            }
            agent->get_queue()->add(previous->get(), agent->get_queue()->get_current());
        }
    }
}