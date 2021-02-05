#include "config.hpp"

namespace CodeNect
{
std::string Config::app_title;
int Config::win_width;
int Config::win_height;
int Config::vsync;
ImVec4 Config::clear_color;
const char* Config::config_filename = "config.ini";

int Config::Sidebar_c::pad_x;
vec_filenames Config::Sidebar_c::images_filenames;

int Config::init()
{
PLOGI << "Reading config file: " << Config::config_filename;
INIReader reader(Config::config_filename);

if (reader.ParseError() != 0)
{
	PLOGE << "Can't load config file: " << Config::config_filename;
	return RES_FAIL;
}

	Config::app_title = reader.Get("general", "title", "CodeNect");
	Config::win_width = reader.GetInteger("general", "width", 1024);
	Config::win_height = reader.GetInteger("general", "height", 720);
	Config::vsync = reader.GetInteger("general", "vsync", 1);

	const float r = reader.GetReal("clear_color", "r", 0.45f);
	const float g = reader.GetReal("clear_color", "g", 0.55f);
	const float b = reader.GetReal("clear_color", "b", 0.60f);
	const float a = reader.GetReal("clear_color", "a", 1.00f);
	Config::clear_color = ImVec4(r, g, b, a);

	const pair_key_filename test = std::make_pair("test", reader.Get("sidebar_images", "test", "???"));
	const pair_key_filename test2 = std::make_pair("test2", reader.Get("sidebar_images", "test2", "???"));
	Config::Sidebar_c::images_filenames.push_back(test);
	Config::Sidebar_c::images_filenames.push_back(test2);

	Config::Sidebar_c::pad_x = reader.GetInteger("sidebar", "pad_x", 8);

	PLOGI << "Config loaded:";
	PLOGI << "[general]";
	PLOGI << "\tTitle = " << Config::app_title;
	PLOGI << "\tWidth = " << Config::win_width;
	PLOGI << "\tHeight = " << Config::win_height;
	PLOGI << "\tVsync = " << Config::vsync;
	PLOGI << "[clear_color]";
	PLOGI << "\tclear_color.r = " << Config::clear_color.x;
	PLOGI << "\tclear_color.g = " << Config::clear_color.y;
	PLOGI << "\tclear_color.b = " << Config::clear_color.z;
	PLOGI << "\tclear_color.a = " << Config::clear_color.w;
	PLOGI << "[sidebar]";
	PLOGI << "\tpad_x = " << Config::Sidebar_c::pad_x;

	return RES_SUCCESS;
}
}
