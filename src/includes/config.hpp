#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <utility>
#include "SimpleIni.h"
#include "imgui.h"
#include "plog/Log.h"

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
	static std::string style;

	struct Sidebar_c
	{
		static ImVec2 pos;
		static ImVec2 size;
		static ImVec2 max_img_size;
		static ImVec2 padding;
		static ImVec2 item_spacing;
		static vec_filenames images_filenames;
		static vec_filenames images_filenames_hover;

		static std::string indicator_filename;
		static ImVec2 indicator_size;
	};

	static int init();
	static void init_general(CSimpleIniA& reader);
	static void init_sidebar(CSimpleIniA& reader);
};

typedef CodeNect::Config::Sidebar_c Sidebar_c;
}

#endif //CONFIG_HPP
