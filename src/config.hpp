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
		static int pad_x;
		static vec_filenames images_filenames;
	};

	static int init();
};
}

#endif //CONFIG_H
