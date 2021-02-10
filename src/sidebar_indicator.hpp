#ifndef SIDEBAR_INDICATOR_H
#define SIDEBAR_INDICATOR_H

#include "imgui.h"
#include "plog/Log.h"
#include "config.hpp"
#include "filesystem.hpp"
#include "ui_button.hpp"

namespace CodeNect
{
struct SidebarIndicator
{
	static ImVec2 cur_pos;
	static ImVec2 size;
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_hovered;
	static CodeNect::UI_Button btn;

	static int init();
	static void set_style();
	static void unset_style();
	static void update(float dt);
	static void draw();
};
}

#endif //SIDEBAR_INDICATOR_H
