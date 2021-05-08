#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <ctime>
#include <string>
#include "imgui.h"
#include "ImNodesEz.h"
#include "node/node_def.hpp"

namespace CodeNect::Utils
{
bool exists_in_slots(NODE_SLOT slot, std::vector<NODE_SLOT> slots);
void center_text(const char* txt);
void center_text(const char* txt, bool draw_text);
void center_text(const char* txt, const ImVec2& offset_pos);
void center_text(const char* txt, const ImVec2& offset_pos, bool draw_text);
void help_marker(const char* desc, bool same_line = false);
void display_asterisk(bool ref, const char* str);
void display_asterisk(bool ref);
std::string time_now(void);
void open_url(const char* url);
bool bool_from_string(const char* str);
void string_to_name(std::string& str);
const char* cmp_to_op_str(NODE_CMP&);
}

#endif //_UTILS_HPP
