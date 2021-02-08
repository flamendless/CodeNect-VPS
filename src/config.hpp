#ifndef CONFIG_H
#define CONFIG_H

#include "INIReader.h"
#include "imgui.h"
#include "plog/Log.h"
#include <vector>

#include "defines.hpp"

namespace CodeNect
{
struct Config
{
	static std::string app_title;
	static int win_width;
	static int win_height;
	static int vsync;
	static ImVec4 clear_color;
	static const char* config_filename;

	struct Sidebar_c
	{
		static ImVec2 pos;
		static ImVec2 size;
		static ImVec2 padding;
		static ImVec2 item_spacing;
		static vec_filenames images_filenames;
		static vec_filenames images_filenames_hover;
	};

	static int init();
	static void init_general(INIReader& reader);
	static void init_sidebar(INIReader& reader);
};
}

#endif //CONFIG_H
