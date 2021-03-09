#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include "imgui.h"
#include "SimpleIni.h"
#include "core/defines.hpp"

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
	static std::string font;
	static int font_size;
	static CSimpleIniA reader;
	static const char* styles;
	static const char* fonts[6];

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

	struct NodeInterface_c
	{
		static ImVec2 pos;
	};

	static int init(void);
	static bool load_user_config(void);
	static bool load_default_config(void);
	static void init_general(void);
	static void init_sidebar(void);
	static void init_node_interface(void);
	static void update_style(const int style_idx);
	static void update_font(const std::string& font);
	static void update_font_size(const std::string& font_size);
	static bool save_user_config(void);
	static bool reset(void);
};

typedef CodeNect::Config::Sidebar_c Sidebar_c;
typedef CodeNect::Config::NodeInterface_c NodeInterface_c;
}

#endif //CONFIG_HPP