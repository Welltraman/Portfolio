#pragma once
#include "string_extended.hpp"

String replace_outside_quote(const String &a_text, const String &a_replacee, const String &a_replacer) {
	Vector<String> vector = a_text.split(a_replacee);
	if (vector.size() == 1)
		return a_text;
	String string = vector[0];
	for (int i = 0; i < vector.size() - 1; i++) {
		int count = string.count(String::chr(TK_QUOTE));
		if (count % 2 == 0)
			string += a_replacer + vector[i + 1];
		else
			string += a_replacee + vector[i + 1];
	}
	return string;
}

void _connect_strings(Vector<String>& r_vector, int& r_int) {
	r_vector.write[r_int] += r_vector[r_int + 1];
	r_vector.remove(r_int + 1);
	r_int--;
}

Vector<String> split_outside_quote(const String &a_text, const String &a_splitter, const bool a_unquote) {
	Vector<String> vector = a_text.split(a_splitter);
	for (int i = 0; i < vector.size(); i++) {
		vector.write[i] = vector[i].strip_edges();
		if (!vector[i].is_enclosed_in("\"") && vector[i].begins_with(String::chr(TK_QUOTE)) && i < vector.size() - 1)
			_connect_strings(vector, i);
	}
	if (a_unquote) for (int i = 0; i < vector.size(); i++) vector.write[i] = vector[i].unquote();
	return vector;
}

Vector<String> split_outside_quote_and_brackets(const String &a_text) {
	Vector<String> vector = split_outside_quote(a_text, String::chr(TK_COMMA), false);
	for (int i = 0; i < vector.size(); i++) {
		if (!vector[i].ends_with(String::chr(TK_ARRAY_CLOSE)) && vector[i].begins_with(String::chr(TK_ARRAY_OPEN)) && i < vector.size() - 1)
			_connect_strings(vector, i);
		else if (!vector[i].ends_with(String::chr(TK_VECTOR_CLOSE)) && vector[i].begins_with(String::chr(TK_VECTOR_OPEN)) && i < vector.size() - 1)
			_connect_strings(vector, i);
	}
	for (int i = 0; i < vector.size(); i++) vector.write[i] = vector[i].unquote();
	return vector;
}

Array _convert_to_array(const String& a_text) {
	Array arr;
	Vector<String> vector = split_outside_quote_and_brackets(a_text);
	for (int i = 0; i < vector.size(); i++) arr.push_back(convert_string(vector[i]));
	return arr;
}

Vector2 _convert_to_vector2(const Vector<String> &a_vector) {
	Vector2 vector2;
	vector2.x = convert_string(a_vector[0]);
	vector2.y = convert_string(a_vector[1]);
	return vector2;
}

Vector3 _convert_to_vector3(const Vector<String>& a_vector) {
	Vector3 vector3;
	vector3.x = convert_string(a_vector[0]);
	vector3.y = convert_string(a_vector[1]);
	vector3.z = convert_string(a_vector[2]);
	return vector3;
}

Variant _convert_to_vector(const String& a_text) {
	Variant variant;
	Vector<String> vector = split_outside_quote_and_brackets(a_text);
	if (vector.size() == 2)
		variant = _convert_to_vector2(vector);
	else if (vector.size() == 3)
		variant = _convert_to_vector3(vector);
	return variant;
}

Variant convert_string(const String &a_text) {
	if (a_text.is_valid_integer())
		return a_text.to_int();
	if (a_text.is_valid_float())
		return a_text.to_float();
	if (a_text.to_lower() == "true")
		return true;
	if (a_text.to_lower() == "false")
		return false;
	if (a_text.begins_with(String::chr(TK_ARRAY_OPEN)) && a_text.ends_with(String::chr(TK_ARRAY_CLOSE)))
		return _convert_to_array(a_text.substr(1, a_text.length() - 2));
	if (a_text.begins_with(String::chr(TK_VECTOR_OPEN)) && a_text.ends_with(String::chr(TK_VECTOR_CLOSE)))
		return _convert_to_vector(a_text.substr(1, a_text.length() - 2));
	if (a_text.is_quoted())
		return a_text.substr(1, a_text.length() - 2);
	return a_text;
}
