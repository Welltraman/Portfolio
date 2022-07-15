#pragma once
#include "core/ustring.h"
#include "core/variant.h"

enum {
	TK_EQUAL = '=',
	TK_COMMA = ',',
	TK_ARRAY_OPEN = '[',
	TK_ARRAY_CLOSE = ']',
	TK_VECTOR_OPEN = '(',
	TK_VECTOR_CLOSE = ')',
	TK_QUOTE = '"',
};

String replace_outside_quote(const String &a_text, const String &a_replacee, const String &a_replacer);
Vector<String> split_outside_quote(const String &a_text, const String &a_splitter, const bool a_unquote = true);
Vector<String> split_outside_quote_and_brackets(const String &a_text);
Variant convert_string(const String &a_text);
