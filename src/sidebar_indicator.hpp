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
	static CodeNect::UI_Button btn;
	static ImGuiWindowFlags flags;
	static bool is_open;
	static float alpha;

	static int init();
	static void set_style();
	static void unset_style();
	static void update(float dt);
	static void draw();
};
}

#endif //SIDEBAR_INDICATOR_H
