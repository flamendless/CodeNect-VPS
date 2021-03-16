#ifndef _HELP_HPP
#define _HELP_HPP

#include <vector>
#include <string>
#include "IconsFontAwesome5.h"
#include "imgui.h"

namespace CodeNect
{
struct Help
{
	typedef std::vector<std::pair<const char*, const char*>> pair_t;
	typedef std::vector<std::tuple<const char*, const char*, const char*>> tuple_t;

	static ImGuiWindowFlags flags;
	static bool is_open;
	static const char* title;
	static pair_t v_items;
	static tuple_t v_key_items;

	Help() = delete;
	static void register_commands(void);
	static void open(void);
	static void draw(void);
	static void draw_commands(void);
	static void draw_support(void);
};
}

#endif //_HELP_HPP
