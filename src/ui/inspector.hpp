#ifndef _INSPECTOR_HPP
#define _INSPECTOR_HPP

#include "imgui.h"
#include "node/node.hpp"

namespace CodeNect
{
struct Inspector
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;

	Inspector() = delete;
	static int init(void);
	static void register_commands(void);
	static void toggle(void);
	static bool keypress(int key, int scancode, int mods);
	static void draw(void);
	static void draw_all(void);
	static void draw_variables(void);
	static void draw_ds(void);
};
}

#endif //_INSPECTOR_HPP
