#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <vector>
#include "imgui.h"
#include "plog/Log.h"
#include "ppk_assert.h"
#include "filesystem.hpp"
#include "config.hpp"
#include "defines.hpp"
#include "image.hpp"
#include "ui_button.hpp"

namespace CodeNect
{
struct Sidebar
{
	// static map_images images;
	// static map_images images_hover;
	static map_ui_buttons ui_buttons;
	static map_tooltips tooltips;
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_hovered;
	static ImVec2 cur_pos;

	static int init();
	static int load_images();
	static void check_image_size(const CodeNect::Image& img);
	static void set_style();
	static void unset_style();
	static void update(float dt);
	static void draw();
};
}

#endif //SIDEBAR_H
