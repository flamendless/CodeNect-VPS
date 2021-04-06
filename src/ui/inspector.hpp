#ifndef _INSPECTOR_HPP
#define _INSPECTOR_HPP

#include "imgui.h"

namespace CodeNect
{
struct Inspector
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;

	static int init(void);
	static void register_commands(void);
	static void open(void);
	static void draw(void);
	static void draw_variables(void);
	static void draw_ds(void);
};
}

#endif //_INSPECTOR_HPP
