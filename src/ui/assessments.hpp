#ifndef _ASSESSMENTS_UI_HPP
#define _ASSESSMENTS_UI_HPP

#include "imgui.h"
#include "modules/assessments.hpp"

namespace CodeNect
{
struct AssessmentsUI
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_open_assessment;
	static ImVec2 pos;
	static ImVec2 size;

	AssessmentsUI() = delete;
	static int init(void);
	static void register_commands(void);
	static bool keypress(int key, int scancode, int mods);
	static void toggle(void);
	static void draw(void);
	static void draw_assessment(void);
};
}

#endif //_ASSESSMENTS_UI_HPP
