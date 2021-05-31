#ifndef _INFO_HPP
#define _INFO_HPP

#include "imgui.h"

namespace CodeNect
{
struct Info
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;

	static int init(void);
	static void draw(void);
};
}

#endif //_INFO_HPP
