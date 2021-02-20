#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "imgui.h"
#include "SimpleIni.h"
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
	static const char* user_config_filename;
	static std::string style;
	static CSimpleIniA reader;

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
	static bool load_user_config();
	static bool load_default_config();
	static void init_general();
	static void init_sidebar();
	static void update_style(const int style_idx);
	static bool save_user_config();
};

typedef CodeNect::Config::Sidebar_c Sidebar_c;
}

#endif //CONFIG_HPP
