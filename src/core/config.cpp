#include "config.hpp"

#include <utility>
#include "plog/Log.h"

namespace CodeNect
{
//General
std::string Config::app_title;
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

const char* Config::styles = "Dark\0Light\0Classic\0";
const char* Config::fonts[6] = {
	"ProggyClean", "ProggyTiny", "DroidSans",
	"CousineRegular", "KarlaRegular", "RobotoMedium"
};
const char* Config::version;

//Sidebar_c
int Config::Sidebar_c::fade_in;
int Config::Sidebar_c::fade_out;
ImVec2 Config::Sidebar_c::size;
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
ImVec4 Config::NodeInterface_c::label_color = ImVec4(1, 1, 0, 1);

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

	PLOGI << "Config loaded";

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

void Config::init_general(void)
{
	Config::version = ini.GetValue("general", "version", "???");
	Config::app_title = ini.GetValue("general", "title", "CodeNect");
	Config::win_width = std::stoi(ini.GetValue("general", "width", "1024"));
	Config::win_height = std::stoi(ini.GetValue("general", "height", "720"));
	Config::vsync = std::stoi(ini.GetValue("general", "vsync", "1"));

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

	const pair_key_filename project = std::make_pair("project", ini.GetValue("sidebar_images", "project", "???"));
	const pair_key_filename run = std::make_pair("run", ini.GetValue("sidebar_images", "run", "???"));
	const pair_key_filename settings = std::make_pair("settings", ini.GetValue("sidebar_images", "settings", "???"));
	const pair_key_filename about = std::make_pair("about", ini.GetValue("sidebar_images", "about", "???"));
	const pair_key_filename help = std::make_pair("help", ini.GetValue("sidebar_images", "help", "???"));
	images_filenames.push_back(project);
	images_filenames.push_back(run);
	images_filenames.push_back(settings);
	images_filenames.push_back(about);
	images_filenames.push_back(help);

	const pair_key_filename project_hover = std::make_pair("project_hover", ini.GetValue("sidebar_images", "project_hover", "???"));
	const pair_key_filename run_hover = std::make_pair("run_hover", ini.GetValue("sidebar_images", "run_hover", "???"));
	const pair_key_filename settings_hover = std::make_pair("settings_hover", ini.GetValue("sidebar_images", "settings_hover", "???"));
	const pair_key_filename about_hover = std::make_pair("about_hover", ini.GetValue("sidebar_images", "about_hover", "???"));
	const pair_key_filename help_hover = std::make_pair("help_hover", ini.GetValue("sidebar_images", "help_hover", "???"));
	images_filenames_hover.push_back(project_hover);
	images_filenames_hover.push_back(run_hover);
	images_filenames_hover.push_back(settings_hover);
	images_filenames_hover.push_back(about_hover);
	images_filenames_hover.push_back(help_hover);

	const int fade_in = std::stoi(ini.GetValue("sidebar_handler", "fade_in", "-1"));
	const int fade_out = std::stoi(ini.GetValue("sidebar_handler", "fade_out", "-1"));
	const int x = std::stoi(ini.GetValue("sidebar", "pos_x", "-1"));
	const int y = std::stoi(ini.GetValue("sidebar", "pos_y", "-1"));
	const int w = std::stoi(ini.GetValue("sidebar", "width", "-1"));
	const int h = std::stoi(ini.GetValue("sidebar", "height", "-1"));
	const int px = std::stoi(ini.GetValue("sidebar", "pad_x", "-1"));
	const int py = std::stoi(ini.GetValue("sidebar", "pad_y", "-1"));
	const int isx = std::stoi(ini.GetValue("sidebar", "item_spacing_x", "-1"));
	const int isy = std::stoi(ini.GetValue("sidebar", "item_spacing_y", "-1"));
	const int max_img_width = std::stoi(ini.GetValue("sidebar", "max_img_width", "-1"));
	const int max_img_height = std::stoi(ini.GetValue("sidebar", "max_img_height", "-1"));

	Config::Sidebar_c::fade_in = fade_in;
	Config::Sidebar_c::fade_out = fade_out;
	Config::Sidebar_c::pos = ImVec2(x, y);
	Config::Sidebar_c::size = ImVec2(w, h);
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
	const float lc_r = std::stof(ini.GetValue("node_interface", "lc_r", "1"));
	const float lc_g = std::stof(ini.GetValue("node_interface", "lc_g", "1"));
	const float lc_b = std::stof(ini.GetValue("node_interface", "lc_b", "0"));

	Config::NodeInterface_c::pos = ImVec2(x, y);
	Config::NodeInterface_c::label_color = ImVec4(lc_r, lc_g, lc_b, 1);
}

void Config::update_style(const int style_idx)
{
	switch (style_idx)
	{
		case 0: ini.SetValue("general", "style", "dark"); break;
		case 1: ini.SetValue("general", "style", "light"); break;
		case 2: ini.SetValue("general", "style", "classic"); break;
	}
}

void Config::update_font(const std::string& font)
{
	ini.SetValue("general", "font", font.c_str());
}

void Config::update_font_size(const std::string& font_size)
{
	ini.SetValue("general", "font_size", font_size.c_str());
}

void Config::update_command_palette(const ImVec2& size)
{
	std::string width = std::to_string(size.x);
	std::string height = std::to_string(size.y);

	ini.SetValue("command_palette", "width", width.c_str());
	ini.SetValue("command_palette", "height", height.c_str());

	Config::CommandPalette_c::size = size;
}

void Config::update_sidebar(const int fade_in, const int fade_out)
{
	std::string str_fade_in = std::to_string(fade_in);
	std::string str_fade_out = std::to_string(fade_out);

	ini.SetValue("sidebar_handler", "fade_in", str_fade_in.c_str());
	ini.SetValue("sidebar_handler", "fade_out", str_fade_out.c_str());

	Config::Sidebar_c::fade_in = fade_in;
	Config::Sidebar_c::fade_out = fade_out;
}

void Config::update_node_interface(float* label_color)
{
	float lc_r = label_color[0];
	float lc_g = label_color[1];
	float lc_b = label_color[2];
	std::string str_lc_r = std::to_string(lc_r);
	std::string str_lc_g = std::to_string(lc_g);
	std::string str_lc_b = std::to_string(lc_b);

	ini.SetValue("node_interface", "lc_r", str_lc_r.c_str());
	ini.SetValue("node_interface", "lc_g", str_lc_g.c_str());
	ini.SetValue("node_interface", "lc_b", str_lc_b.c_str());

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
		PLOGV << "Saving user config: " << res;

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

void Config::Sidebar_c::shutdown(void)
{
	PLOGI << "Clearing sidebar_c...";

	Sidebar_c::images_filenames.clear();
	Sidebar_c::images_filenames_hover.clear();

	PLOGI << "Cleared sidebar_c successfully";
}
}
