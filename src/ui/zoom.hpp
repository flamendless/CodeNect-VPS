#ifndef _ZOOM_HPP
#define _ZOOM_HPP

#include "imgui.h"

namespace CodeNect
{
struct Zoom
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;
	static float zoom_factor;

	static int init(void);
	static void register_commands(void);
	static void open(void);
	static void close(void);
	static void draw(void);
};
}

#endif //_ZOOM_HPP
