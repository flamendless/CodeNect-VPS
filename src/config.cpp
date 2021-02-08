#include "config.hpp"

namespace CodeNect
{
std::string Config::app_title;
int Config::win_width;
int Config::win_height;
int Config::vsync;
ImVec4 Config::clear_color;
const char* Config::config_filename = "config.ini";

ImVec2 Config::Sidebar_c::size;
ImVec2 Config::Sidebar_c::pos;
ImVec2 Config::Sidebar_c::padding;
ImVec2 Config::Sidebar_c::item_spacing;
vec_filenames Config::Sidebar_c::images_filenames;
vec_filenames Config::Sidebar_c::images_filenames_hover;

int Config::init()
{
	PLOGI << "Reading config file: " << Config::config_filename;
	INIReader reader(Config::config_filename);

	if (reader.ParseError() != 0)
	{
		PLOGE << "Can't load config file: " << Config::config_filename;
		return RES_FAIL;
	}

	Config::init_general(reader);
	Config::init_sidebar(reader);

	PLOGI << "Config loaded";

	return RES_SUCCESS;
}

void Config::init_general(INIReader& reader)
{
	Config::app_title = reader.Get("general", "title", "CodeNect");
	Config::win_width = reader.GetInteger("general", "width", 1024);
	Config::win_height = reader.GetInteger("general", "height", 720);
	Config::vsync = reader.GetInteger("general", "vsync", 1);

	const float r = reader.GetReal("clear_color", "r", 0.45f);
	const float g = reader.GetReal("clear_color", "g", 0.55f);
	const float b = reader.GetReal("clear_color", "b", 0.60f);
	const float a = reader.GetReal("clear_color", "a", 1.00f);
	Config::clear_color = ImVec4(r, g, b, a);

}

void Config::init_sidebar(INIReader& reader)
{
	vec_filenames &images_filenames = Config::Sidebar_c::images_filenames;
	vec_filenames &images_filenames_hover = Config::Sidebar_c::images_filenames_hover;

	const pair_key_filename folder = std::make_pair("folder", reader.Get("sidebar_images", "folder", "???"));
	const pair_key_filename run = std::make_pair("run", reader.Get("sidebar_images", "run", "???"));
	images_filenames.push_back(folder);
	images_filenames.push_back(run);

	const pair_key_filename folder_hover = std::make_pair("folder_hover", reader.Get("sidebar_images", "folder_hover", "???"));
	const pair_key_filename run_hover = std::make_pair("run_hover", reader.Get("sidebar_images", "run_hover", "???"));
	images_filenames_hover.push_back(folder_hover);
	images_filenames_hover.push_back(run_hover);

	const int sidebar_x = reader.GetInteger("sidebar", "pos_x", -1);
	const int sidebar_y = reader.GetInteger("sidebar", "pos_y", -1);
	const int sidebar_w = reader.GetInteger("sidebar", "width", -1);
	const int sidebar_h = reader.GetInteger("sidebar", "height", -1);
	const int sidebar_px = reader.GetInteger("sidebar", "pad_x", -1);
	const int sidebar_py = reader.GetInteger("sidebar", "pad_y", -1);
	const int sidebar_isx = reader.GetInteger("sidebar", "item_spacing_x", -1);
	const int sidebar_isy = reader.GetInteger("sidebar", "item_spacing_y", -1);

	Config::Sidebar_c::pos = ImVec2(sidebar_x, sidebar_y);
	Config::Sidebar_c::size = ImVec2(sidebar_w, sidebar_h);
	Config::Sidebar_c::padding = ImVec2(sidebar_px, sidebar_py);
	Config::Sidebar_c::item_spacing = ImVec2(sidebar_isx, sidebar_isy);

}
}
