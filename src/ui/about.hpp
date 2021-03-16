#ifndef _ABOUT_HPP
#define _ABOUT_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"

namespace CodeNect
{
struct About
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static const char* title;

	About() = delete;
	static void register_commands(void);
	static void open(void);
	static void draw(void);
};
}

#endif //_ABOUT_HPP
