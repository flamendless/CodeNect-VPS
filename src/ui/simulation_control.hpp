#ifndef _SIMULATION_CONTROL_HPP
#define _SIMULATION_CONTROL_HPP

#include "imgui.h"

namespace CodeNect
{
struct SimulationControl
{
	static const char* title;
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;

	SimulationControl() = delete;
	static int init(void);
	static void register_commands(void);
	static void toggle(void);
	static void draw(void);
};
}

#endif //_SIMULATION_CONTROL_HPP
