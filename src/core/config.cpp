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
CSimpleIniA Config::reader;

const char* Config::styles = "Dark\0Light\0Classic\0";
const char* Config::fonts[6] = {
	"ProggyClean", "ProggyTiny", "DroidSans",
	"CousineRegular", "KarlaRegular", "RobotoMedium"
};
const char* Config::version;

//Sidebar_c
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

int Config::init(void)
{
	reader.SetUnicode();

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
	SI_Error res = reader.LoadFile(Config::user_config_filename);

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

	SI_Error res = reader.LoadFile(Config::config_filename);

	if (res < 0)
	{
		PLOGI << "Default config file not found";

		return false;
	}

	return true;
}

void Config::init_general(void)
{
	Config::version = reader.GetValue("general", "version", "???");
	Config::app_title = reader.GetValue("general", "title", "CodeNect");
	Config::win_width = std::stoi(reader.GetValue("general", "width", "1024"));
	Config::win_height = std::stoi(reader.GetValue("general", "height", "720"));
	Config::vsync = std::stoi(reader.GetValue("general", "vsync", "1"));

	const float r = std::stof(reader.GetValue("clear_color", "r", "0.45f"));
	const float g = std::stof(reader.GetValue("clear_color", "g", "0.55f"));
	const float b = std::stof(reader.GetValue("clear_color", "b", "0.60f"));
	const float a = std::stof(reader.GetValue("clear_color", "a", "1.00f"));
	Config::clear_color = ImVec4(r, g, b, a);

	Config::style = reader.GetValue("general", "style", DEFAULT_STYLE);
	Config::font = reader.GetValue("general", "font", DEFAULT_FONT);
	Config::font_size = std::stoi(reader.GetValue("general", "font_size", std::to_string(DEFAULT_FONT_SIZE).c_str()));
}

void Config::init_sidebar(void)
{
	vec_filenames& images_filenames = Config::Sidebar_c::images_filenames;
	vec_filenames& images_filenames_hover = Config::Sidebar_c::images_filenames_hover;

	const pair_key_filename project = std::make_pair("project", reader.GetValue("sidebar_images", "project", "???"));
	const pair_key_filename run = std::make_pair("run", reader.GetValue("sidebar_images", "run", "???"));
	const pair_key_filename settings = std::make_pair("settings", reader.GetValue("sidebar_images", "settings", "???"));
	const pair_key_filename about = std::make_pair("about", reader.GetValue("sidebar_images", "about", "???"));
	images_filenames.push_back(project);
	images_filenames.push_back(run);
	images_filenames.push_back(settings);
	images_filenames.push_back(about);

	const pair_key_filename project_hover = std::make_pair("project_hover", reader.GetValue("sidebar_images", "project_hover", "???"));
	const pair_key_filename run_hover = std::make_pair("run_hover", reader.GetValue("sidebar_images", "run_hover", "???"));
	const pair_key_filename settings_hover = std::make_pair("settings_hover", reader.GetValue("sidebar_images", "settings_hover", "???"));
	const pair_key_filename about_hover = std::make_pair("about_hover", reader.GetValue("sidebar_images", "about_hover", "???"));
	images_filenames_hover.push_back(project_hover);
	images_filenames_hover.push_back(run_hover);
	images_filenames_hover.push_back(settings_hover);
	images_filenames_hover.push_back(about_hover);

	const int x = std::stoi(reader.GetValue("sidebar", "pos_x", "-1"));
	const int y = std::stoi(reader.GetValue("sidebar", "pos_y", "-1"));
	const int w = std::stoi(reader.GetValue("sidebar", "width", "-1"));
	const int h = std::stoi(reader.GetValue("sidebar", "height", "-1"));
	const int px = std::stoi(reader.GetValue("sidebar", "pad_x", "-1"));
	const int py = std::stoi(reader.GetValue("sidebar", "pad_y", "-1"));
	const int isx = std::stoi(reader.GetValue("sidebar", "item_spacing_x", "-1"));
	const int isy = std::stoi(reader.GetValue("sidebar", "item_spacing_y", "-1"));
	const int max_img_width = std::stoi(reader.GetValue("sidebar", "max_img_width", "-1"));
	const int max_img_height = std::stoi(reader.GetValue("sidebar", "max_img_height", "-1"));

	Config::Sidebar_c::pos = ImVec2(x, y);
	Config::Sidebar_c::size = ImVec2(w, h);
	Config::Sidebar_c::max_img_size = ImVec2(max_img_width, max_img_height);
	Config::Sidebar_c::padding = ImVec2(px, py);
	Config::Sidebar_c::item_spacing = ImVec2(isx, isy);

	//sidebar indicator
	const std::string indicator_filename = reader.GetValue("sidebar_indicator", "indicator", "???");
	const int siw = std::stoi(reader.GetValue("sidebar_indicator", "width", "-1"));
	const int sih = std::stoi(reader.GetValue("sidebar_indicator", "height", "-1"));

	Config::Sidebar_c::indicator_filename = indicator_filename;
	Config::Sidebar_c::indicator_size = ImVec2(siw, sih);
}

void Config::init_command_palette(void)
{
	const int width = std::stoi(reader.GetValue("command_palette", "width", "-1"));
	const int height = std::stoi(reader.GetValue("command_palette", "height", "-1"));

	Config::CommandPalette_c::size = ImVec2(width, height);
}

void Config::init_node_interface(void)
{
	const int x = std::stoi(reader.GetValue("node_interface", "pos_x", "-1"));
	const int y = std::stoi(reader.GetValue("node_interface", "pos_y", "-1"));

	Config::NodeInterface_c::pos = ImVec2(x, y);
}

void Config::update_style(const int style_idx)
{
	switch (style_idx)
	{
		case 0: reader.SetValue("general", "style", "dark"); break;
		case 1: reader.SetValue("general", "style", "light"); break;
		case 2: reader.SetValue("general", "style", "classic"); break;
	}
}

void Config::update_font(const std::string& font)
{
	reader.SetValue("general", "font", font.c_str());
}

void Config::update_font_size(const std::string& font_size)
{
	reader.SetValue("general", "font_size", font_size.c_str());
}

void Config::update_command_palette(const ImVec2& size)
{
	std::string width = std::to_string(size.x);
	std::string height = std::to_string(size.y);

	reader.SetValue("command_palette", "width", width.c_str());
	reader.SetValue("command_palette", "height", height.c_str());

	Config::CommandPalette_c::size = size;
}

bool Config::save_user_config(void)
{
	bool res = reader.SaveFile(Config::user_config_filename);

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
