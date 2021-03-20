#ifndef _COMMAND_PALETTE_HPP
#define _COMMAND_PALETTE_HPP

#include <functional>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "core/commands.hpp"

namespace CodeNect
{
struct CommandPalette
{
	static bool is_open;
	static const char* title;
	static const char* str_close;
	static ImVec2 size;
	static ImGuiWindowFlags flags;
	static ImGuiTextFilter filter;
	static int cur_pos;
	static int cur_cmd;
	static int filtered;

	CommandPalette() = delete;
	static int init(void);
	static void open(void);
	static void close(void);
	static bool keypress(int key, int scancode, int mods);
	static void draw(void);
};
}

#endif //_COMMAND_PALETTE_HPP
