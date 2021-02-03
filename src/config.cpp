#include "config.hpp"

namespace CodeNect
{
std::string Config::app_title;
int Config::win_width;
int Config::win_height;
int Config::vsync;
ImVec4 Config::clear_color;
const char* Config::config_filename = "config.ini";
const std::vector<std::string> Config::sidebar_images_filenames {
		"test.png",
	};

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

	float r = reader.GetReal("clear_color", "r", 0.45f);
	float g = reader.GetReal("clear_color", "g", 0.55f);
	float b = reader.GetReal("clear_color", "b", 0.60f);
	float a = reader.GetReal("clear_color", "a", 1.00f);

	Config::clear_color = ImVec4(r, g, b, a);

	PLOGI << "Config loaded:";
	PLOGI << "\t[general]";
	PLOGI << "\tTitle = " << Config::app_title;
	PLOGI << "\tWidth = " << Config::win_width;
	PLOGI << "\tHeight = " << Config::win_height;
	PLOGI << "\tVsync = " << Config::vsync;
	PLOGI << "\t[clear_color]";
	PLOGI << "\tclear_color.r = " << Config::clear_color.x;
	PLOGI << "\tclear_color.g = " << Config::clear_color.y;
	PLOGI << "\tclear_color.b = " << Config::clear_color.z;
	PLOGI << "\tclear_color.a = " << Config::clear_color.w;

	return RES_SUCCESS;
}
}
