#ifndef _TERMINAL_HPP
#define _TERMINAL_HPP

#include "imgui.h"

namespace CodeNect
{
struct Terminal
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;

	Terminal() = delete;
	static int init(void);
	static void register_commands(void);
	static void toggle(void);
	static void draw(void);
};
}

#endif //_TERMINAL_HPP
