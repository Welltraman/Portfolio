#pragma once
#include "id8.hpp"

uint8_t ID8::m_max_id = 0;
std::queue<uint8_t> ID8::m_returned_ids = std::queue<uint8_t>();
