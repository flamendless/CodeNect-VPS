#ifndef ABOUT_HPP
#define ABOUT_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"

namespace CodeNect
{
struct About
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static const char* title;

	static void register_commands(void);
	static void open(void);
	static void draw(void);
};
}

#endif //ABOUT_HPP
