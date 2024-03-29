#include "config.hpp"

#include <utility>
#include <GLFW/glfw3.h>
#include "plog/Log.h"
#include "core/utils.hpp"

namespace CodeNect
{
//General
const char* Config::terminal;
bool Config::fullscreen = false;
int Config::win_width;
int Config::win_height;
int Config::vsync;
ImVec4 Config::clear_color;
const char* Config::config_filename = "config.ini";
const char* Config::user_config_filename = "config_user.ini";
std::string Config::style;
std::string Config::font;
int Config::font_size;
CSimpleIniA Config::ini;

const char* Config::fonts[6] = {
	"ProggyClean", "ProggyTiny", "DroidSans",
	"CousineRegular", "KarlaRegular", "RobotoMedium"
};
const char* Config::version;
bool Config::first_time = true;
bool Config::tutorial_done = false;

//Sidebar_c
int Config::Sidebar_c::fade_in;
int Config::Sidebar_c::fade_out;
int Config::Sidebar_c::width;
ImVec2 Config::Sidebar_c::max_img_size;
ImVec2 Config::Sidebar_c::pos;
ImVec2 Config::Sidebar_c::padding;
ImVec2 Config::Sidebar_c::item_spacing;
vec_filenames Config::Sidebar_c::images_filenames;
vec_filenames Config::Sidebar_c::images_filenames_hover;
std::string Config::Sidebar_c::indicator_filename;
ImVec2 Config::Sidebar_c::indicator_size;

//CommandPalette_c
ImVec2 Config::CommandPalette_c::size;

//NodeInterface_c
ImVec2 Config::NodeInterface_c::pos;
ImVec2 Config::NodeInterface_c::item_inner_spacing = ImVec2(8, 8);
ImVec4 Config::NodeInterface_c::color_dark = ImVec4(1, 1, 0, 1);
ImVec4 Config::NodeInterface_c::color_light = ImVec4(1, 0, 0, 1);
ImVec4 Config::NodeInterface_c::title_color = Config::NodeInterface_c::color_dark;
ImVec4 Config::NodeInterface_c::label_color = Config::NodeInterface_c::color_dark;

int Config::init(void)
{
	ini.SetUnicode();

	bool res = Config::load_user_config();

	if (!res)
	{
		bool res2 = Config::load_default_config();

		if (!res2)
			return RES_FAIL;
	}

	Config::init_general();
	Config::init_sidebar();
	Config::init_command_palette();
	Config::init_node_interface();

	PLOGI << "Config loaded:";
	PLOGI << "\tVersion" << Config::version;
	PLOGI << "\tWindow width: " << Config::win_width;
	PLOGI << "\tWindow height: " << Config::win_height;
	PLOGI << "\tFullscreen: " << Config::fullscreen;

	return RES_SUCCESS;
}

bool Config::load_user_config(void)
{
	PLOGI << "Reading user config file: " << Config::user_config_filename;
	SI_Error res = ini.LoadFile(Config::user_config_filename);

	if (res < 0)
	{
		PLOGI << "User config file not found";

		return false;
	}

	return true;
}

bool Config::load_default_config(void)
{
	PLOGI << "Reading default config file: " << Config::config_filename;

	SI_Error res = ini.LoadFile(Config::config_filename);

	if (res < 0)
	{
		PLOGI << "Default config file not found";

		return false;
	}

	return true;
}

void Config::set_style(int idx)
{
	if (idx == -1)
		idx = (STYLE::_from_string(Config::style.c_str()))._to_index();

	if (idx == 1)
	{
		Config::NodeInterface_c::title_color = Config::NodeInterface_c::color_dark;
		Config::NodeInterface_c::label_color = Config::NodeInterface_c::color_dark;
		ImGui::StyleColorsDark();
	}
	else if (idx == 2)
	{
		Config::NodeInterface_c::title_color = Config::NodeInterface_c::color_light;
		Config::NodeInterface_c::label_color = Config::NodeInterface_c::color_light;
		ImGui::StyleColorsLight();
	}
	else if (idx == 3)
	{
		Config::NodeInterface_c::title_color = Config::NodeInterface_c::color_dark;
		Config::NodeInterface_c::label_color = Config::NodeInterface_c::color_dark;
		ImGui::StyleColorsClassic();
	}
}

void Config::init_general(void)
{
	Config::terminal = ini.GetValue("general", "terminal", "$TERMINAL");
	const char* fullscreen = ini.GetValue("general", "fullscreen", "false");
	Config::fullscreen = Utils::bool_from_string(fullscreen);
	Config::version = ini.GetValue("general", "version", "???");
	Config::win_width = std::stoi(ini.GetValue("general", "width", "1280"));
	Config::win_height = std::stoi(ini.GetValue("general", "height", "720"));
	Config::vsync = std::stoi(ini.GetValue("general", "vsync", "1"));

	const char* first_time = ini.GetValue("general", "first_time", "true");
	Config::first_time = Utils::bool_from_string(first_time);
	const char* tutorial_done = ini.GetValue("general", "tutorial_done", "false");
	Config::tutorial_done = Utils::bool_from_string(tutorial_done);

	const float r = std::stof(ini.GetValue("clear_color", "r", "0.45f"));
	const float g = std::stof(ini.GetValue("clear_color", "g", "0.55f"));
	const float b = std::stof(ini.GetValue("clear_color", "b", "0.60f"));
	const float a = std::stof(ini.GetValue("clear_color", "a", "1.00f"));
	Config::clear_color = ImVec4(r, g, b, a);

	Config::style = ini.GetValue("general", "style", DEFAULT_STYLE);
	Config::font = ini.GetValue("general", "font", DEFAULT_FONT);
	Config::font_size = std::stoi(ini.GetValue("general", "font_size", std::to_string(DEFAULT_FONT_SIZE).c_str()));
}

void Config::init_sidebar(void)
{
	vec_filenames& images_filenames = Config::Sidebar_c::images_filenames;
	vec_filenames& images_filenames_hover = Config::Sidebar_c::images_filenames_hover;

	std::vector<std::string> v_images = {
		"project", "run", "inspector", "docs", "assessment", "settings", "about", "help"
	};
	for (std::string& str : v_images)
	{
		std::string str_h = str;
		str_h.append("_hover");
		const pair_key_filename p = std::make_pair(str, ini.GetValue("sidebar_images", str.c_str(), str.c_str()));
		const pair_key_filename p_hover = std::make_pair(str_h, ini.GetValue("sidebar_images", str_h.c_str(), str_h.c_str()));
		images_filenames.push_back(p);
		images_filenames_hover.push_back(p_hover);
	}

	const int fade_in = std::stoi(ini.GetValue("sidebar_handler", "fade_in", "-1"));
	const int fade_out = std::stoi(ini.GetValue("sidebar_handler", "fade_out", "-1"));
	const int x = std::stoi(ini.GetValue("sidebar", "pos_x", "-1"));
	const int y = std::stoi(ini.GetValue("sidebar", "pos_y", "-1"));
	const int w = std::stoi(ini.GetValue("sidebar", "width", "-1"));
	const int px = std::stoi(ini.GetValue("sidebar", "pad_x", "-1"));
	const int py = std::stoi(ini.GetValue("sidebar", "pad_y", "-1"));
	const int isx = std::stoi(ini.GetValue("sidebar", "item_spacing_x", "-1"));
	const int isy = std::stoi(ini.GetValue("sidebar", "item_spacing_y", "-1"));
	const int max_img_width = std::stoi(ini.GetValue("sidebar", "max_img_width", "-1"));
	const int max_img_height = std::stoi(ini.GetValue("sidebar", "max_img_height", "-1"));

	Config::Sidebar_c::fade_in = fade_in;
	Config::Sidebar_c::fade_out = fade_out;
	Config::Sidebar_c::pos = ImVec2(x, y);
	Config::Sidebar_c::width = w;
	Config::Sidebar_c::max_img_size = ImVec2(max_img_width, max_img_height);
	Config::Sidebar_c::padding = ImVec2(px, py);
	Config::Sidebar_c::item_spacing = ImVec2(isx, isy);

	//sidebar indicator
	const std::string indicator_filename = ini.GetValue("sidebar_indicator", "indicator", "???");
	const int siw = std::stoi(ini.GetValue("sidebar_indicator", "width", "-1"));
	const int sih = std::stoi(ini.GetValue("sidebar_indicator", "height", "-1"));

	Config::Sidebar_c::indicator_filename = indicator_filename;
	Config::Sidebar_c::indicator_size = ImVec2(siw, sih);
}

void Config::init_command_palette(void)
{
	const int width = std::stoi(ini.GetValue("command_palette", "width", "-1"));
	const int height = std::stoi(ini.GetValue("command_palette", "height", "-1"));

	Config::CommandPalette_c::size = ImVec2(width, height);
}

void Config::init_node_interface(void)
{
	const int x = std::stoi(ini.GetValue("node_interface", "pos_x", "-1"));
	const int y = std::stoi(ini.GetValue("node_interface", "pos_y", "-1"));
	const int iis_x = std::stoi(ini.GetValue("node_interface", "item_inner_spacing_x", "8"));
	const int iis_y = std::stoi(ini.GetValue("node_interface", "item_inner_spacing_y", "8"));
	const float lc_r = std::stof(ini.GetValue("node_interface", "lc_r", "1"));
	const float lc_g = std::stof(ini.GetValue("node_interface", "lc_g", "1"));
	const float lc_b = std::stof(ini.GetValue("node_interface", "lc_b", "0"));
	const float tc_r = std::stof(ini.GetValue("node_interface", "tc_r", "1"));
	const float tc_g = std::stof(ini.GetValue("node_interface", "tc_g", "1"));
	const float tc_b = std::stof(ini.GetValue("node_interface", "tc_b", "0"));

	Config::NodeInterface_c::pos = ImVec2(x, y);
	Config::NodeInterface_c::item_inner_spacing = ImVec2(iis_x, iis_y);
	Config::NodeInterface_c::label_color = ImVec4(lc_r, lc_g, lc_b, 1);
	Config::NodeInterface_c::title_color = ImVec4(tc_r, tc_g, tc_b, 1);
}

void Config::update_fullscreen(void)
{
	const char* str = (Config::fullscreen) ? "true" : "false";
	ini.SetValue("general", "fullscreen", str);
}

void Config::update_style(StyleData& style_data)
{
	switch (style_data.style._to_index())
	{
		case 1: ini.SetValue("general", "style", "DARK"); break;
		case 2: ini.SetValue("general", "style", "LIGHT"); break;
		case 3: ini.SetValue("general", "style", "CLASSIC"); break;
	}

	std::string str_font_size = std::to_string(style_data.font_size);

	ini.SetValue("general", "font", style_data.font.c_str());
	ini.SetValue("general", "font_size", str_font_size.c_str());
}

void Config::update_command_palette(CommandPaletteData& cp_data)
{
	std::string width = std::to_string(cp_data.size.x);
	std::string height = std::to_string(cp_data.size.y);

	ini.SetValue("command_palette", "width", width.c_str());
	ini.SetValue("command_palette", "height", height.c_str());

	Config::CommandPalette_c::size = cp_data.size;
}

void Config::update_sidebar(SidebarData& sb_data)
{
	std::string str_fade_in = std::to_string(sb_data.fade_in);
	std::string str_fade_out = std::to_string(sb_data.fade_out);

	ini.SetValue("sidebar_handler", "fade_in", str_fade_in.c_str());
	ini.SetValue("sidebar_handler", "fade_out", str_fade_out.c_str());

	Config::Sidebar_c::fade_in = sb_data.fade_in;
	Config::Sidebar_c::fade_out = sb_data.fade_out;
}

void Config::update_node_interface(NodeInterfaceData& ni_data)
{
	float tc_r = ni_data.title_color[0];
	float tc_g = ni_data.title_color[1];
	float tc_b = ni_data.title_color[2];
	float lc_r = ni_data.label_color[0];
	float lc_g = ni_data.label_color[1];
	float lc_b = ni_data.label_color[2];
	int iis_x = ni_data.item_inner_spacing[0];
	int iis_y = ni_data.item_inner_spacing[1];

	std::string str_iis_x = std::to_string(iis_x);
	std::string str_iis_y = std::to_string(iis_y);
	std::string str_tc_r = std::to_string(tc_r);
	std::string str_tc_g = std::to_string(tc_g);
	std::string str_tc_b = std::to_string(tc_b);
	std::string str_lc_r = std::to_string(lc_r);
	std::string str_lc_g = std::to_string(lc_g);
	std::string str_lc_b = std::to_string(lc_b);

	ini.SetValue("node_interface", "item_inner_spacing_x", str_iis_x.c_str());
	ini.SetValue("node_interface", "item_inner_spacing_y", str_iis_y.c_str());
	ini.SetValue("node_interface", "tc_r", str_tc_r.c_str());
	ini.SetValue("node_interface", "tc_g", str_tc_g.c_str());
	ini.SetValue("node_interface", "tc_b", str_tc_b.c_str());
	ini.SetValue("node_interface", "lc_r", str_lc_r.c_str());
	ini.SetValue("node_interface", "lc_g", str_lc_g.c_str());
	ini.SetValue("node_interface", "lc_b", str_lc_b.c_str());
	Config::NodeInterface_c::item_inner_spacing = ImVec2(iis_x, iis_y);
	Config::NodeInterface_c::title_color = ImVec4(tc_r, tc_g, tc_b, 1);
	Config::NodeInterface_c::label_color = ImVec4(lc_r, lc_g, lc_b, 1);
}

bool Config::save_user_config(void)
{
	bool res = ini.SaveFile(Config::user_config_filename);

	if (res != 0)
	{
		PLOGW << "Failed to save user config: " << res;
		return RES_FAIL;
	}
	else
		PLOGV << "Saving user config: " << ((res == 0) ? "success" : "fail");

	return RES_SUCCESS;
}

bool Config::reset(void)
{
	bool res = std::remove(Config::user_config_filename);

	if (res != 0)
	{
		PLOGW << "Failed to delete user config: " << res;

		return RES_FAIL;
	}
	else
		PLOGV << "Deleted user config: " << res;

	return RES_SUCCESS;
}

void Config::shutdown(void)
{
	PLOGI << "Clearing config...";
	Config::save_user_config();

	Sidebar_c::images_filenames.clear();
	Sidebar_c::images_filenames_hover.clear();

	PLOGI << "Cleared config successfully";
}
}
