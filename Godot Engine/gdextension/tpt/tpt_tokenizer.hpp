#pragma once

namespace godot {

class TPTTokenizer {
public:
    enum TPTToken {
        TK_NULL = NULL,
        TK_BLOCK = ':',
        TK_COMMAND = '/',
        TK_IF = '?',
        TK_DELAY = '~',
        TK_NEXT = '->',
        TK_REPEAT_BEGIN = '[',
        TK_REPEAT_END = ']',
    };
    inline const static char32_t tk_list[] = { TK_BLOCK, TK_COMMAND, TK_IF, TK_DELAY, TK_NEXT, TK_REPEAT_BEGIN, TK_REPEAT_END };
    inline const static std::string tk_str[] = { "", "", "_if_", "_delay_", "_next_", "_repeat_begin_", "_repeat_end_" };
    
    static TPTToken get_token(const String& p_text) {
        char32_t tk = p_text[0];
        if (tk == '-') {
            tk = tk * 256 + p_text[1];
        }
        auto it = std::find(tk_list, tk_list + sizeof(tk_list) / sizeof(tk_list[0]), tk);
        if (it != std::end(tk_list)) {
            return static_cast<TPTToken>(tk);
        }
        return TK_NULL;
    }

    static std::string get_tpt_method_id(const char32_t p_token) {
        auto it = std::find(tk_list, tk_list + sizeof(tk_list) / sizeof(tk_list[0]), p_token);
        if (it != std::end(tk_list)) {
            return tk_str[std::distance(std::begin(tk_list), it)];
        }
        return "";
    }
};

}