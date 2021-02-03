#ifndef CONFIG_H
#define CONFIG_H

#include "imgui.h"

namespace Config
{
static const char* app_title = "CodeNect";
static const int win_width = 1024;
static const int win_height = 720;
static const int vsync = 1;
static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static const std::vector<std::string> sidebar_images_filenames {
		"test.png",
	};
}

#endif //CONFIG_H
