#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include <string>
#include "imgui.h"
#include "SimpleIni.h"
#include "core/defines.hpp"
#include "core/settings_data.hpp"

namespace CodeNect
{
struct Config
{
	static const char* terminal;
	static bool fullscreen;
	static int win_width;
	static int win_height;
	static int vsync;
	static ImVec4 clear_color;
	static const char* config_filename;
	static const char* user_config_filename;
	static std::string style;
	static std::string font;
	static int font_size;
	static CSimpleIniA ini;
	static const char* fonts[6];
	static const char* version;

	struct Sidebar_c
	{
		static int fade_in;
		static int fade_out;
		static ImVec2 pos;
		static int width;
		static ImVec2 max_img_size;
		static ImVec2 padding;
		static ImVec2 item_spacing;
		static vec_filenames images_filenames;
		static vec_filenames images_filenames_hover;

		static std::string indicator_filename;
		static ImVec2 indicator_size;
	};

	struct CommandPalette_c
	{
		static ImVec2 size;
	};

	struct NodeInterface_c
	{
		static ImVec2 pos;
		static ImVec2 item_inner_spacing;
		static ImVec4 color_dark;
		static ImVec4 color_light;
		static ImVec4 label_color;
		static ImVec4 title_color;
	};

	struct Inspector_c
	{
		static ImVec2 padding;
		static ImVec2 size;
	};

	Config() = delete;
	static int init(void);
	static bool load_user_config(void);
	static bool load_default_config(void);
	static void set_style(int idx);
	static void init_general(void);
	static void init_sidebar(void);
	static void init_command_palette(void);
	static void init_node_interface(void);
	static void init_inspector(void);
	static void update_fullscreen(void);
	static void update_style(StyleData& style_data);
	static void update_command_palette(CommandPaletteData& cp_data);
	static void update_sidebar(SidebarData& sb_data);
	static void update_node_interface(NodeInterfaceData& ni_data);
	static bool save_user_config(void);
	static bool reset(void);
	static void shutdown(void);

	typedef Config::Sidebar_c Sidebar_c;
	typedef Config::CommandPalette_c CommandPalette_c;
	typedef Config::NodeInterface_c NodeInterface_c;
	typedef Config::Inspector_c Inspector_c;
};
}

#endif //_CONFIG_HPP
