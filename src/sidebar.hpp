#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "imgui.h"
#include "plog/Log.h"
#include "filesystem.hpp"
#include <vector>
#include "config.hpp"
#include "defines.hpp"
#include "image.hpp"

namespace CodeNect
{
struct Sidebar
{
	static map_images sidebar_images;
	static ImGuiWindowFlags flags;
	static bool is_open;

	static int init();
	static int load_images();
	static void draw();
};
}

#endif //SIDEBAR_H
