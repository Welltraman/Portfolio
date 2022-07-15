#pragma once
#include "print_string_extended.hpp"
#include <queue>

class ID8 {
	uint8_t m_id;
	static uint8_t m_max_id;
	static std::queue<uint8_t> m_returned_ids;
	static uint8_t _allocate_id() {
		if (!m_returned_ids.empty()) {
			uint8_t r = m_returned_ids.front();
			m_returned_ids.pop();
			return r;
		}
		else {
			if (m_max_id == 255) print_err("ID8 Overflow.", ERR_PRINTER_ON_FIRE);
			return m_max_id++;
		}
	}
public:
	bool operator==(const ID8& rhs) { return m_id == rhs.m_id; }
	operator int() const { return m_id; }
	ID8() { m_id = _allocate_id(); }
	~ID8() { m_returned_ids.push(m_id); }
};
