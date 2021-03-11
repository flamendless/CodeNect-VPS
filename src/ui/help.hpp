#ifndef HELP_HPP
#define HELP_HPP

#include <vector>
#include <string>
#include "IconsFontAwesome5.h"
#include "imgui.h"

namespace CodeNect
{
struct Help
{
	typedef std::vector<std::pair<const char*, const char*>> items_t;

	static ImGuiWindowFlags flags;
	static bool is_open;
	static const char* title;
	static items_t v_items;

	Help() = delete;
	static void register_commands(void);
	static void open(void);
	static void draw(void);
	static void draw_support(void);
};
}

#endif //HELP_HPP
