#ifndef _TUTORIAL_HPP
#define _TUTORIAL_HPP

#include <string>
#include <map>
#include "imgui.h"

namespace CodeNect
{
struct Tutorial
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;
	static unsigned int current_step;
	static std::map<const std::string, bool> m_steps;

	static int init(void);
	static void register_commands(void);
	static void toggle(void);
	static void draw(void);
};
}

#endif //_TUTORIAL_HPP
