#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

#include "imgui.h"

namespace CodeNect
{
struct Settings
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_first;
	static bool is_pos_locked;
	static const char* title;

	Settings() = delete;
	static void init(void);
	static void register_commands(void);
	static void open(void);
	static void close(void);
	static void reset_values(void);
	static void draw(void);
	static void draw_window(void);
	static void draw_buttons(void);
	static void draw_theme_select(void);
	static void draw_font_select(void);
	static void draw_sidebar(void);
	static void draw_command_palette(void);
	static void draw_node_interface(void);
};
}

#endif //_SETTINGS_HPP
