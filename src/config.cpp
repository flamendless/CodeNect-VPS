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
ImVec2 Config::Sidebar_c::max_img_size;
ImVec2 Config::Sidebar_c::pos;
ImVec2 Config::Sidebar_c::padding;
ImVec2 Config::Sidebar_c::item_spacing;
vec_filenames Config::Sidebar_c::images_filenames;
vec_filenames Config::Sidebar_c::images_filenames_hover;
std::string Config::Sidebar_c::indicator_filename;
ImVec2 Config::Sidebar_c::indicator_size;

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
	vec_filenames& images_filenames = Config::Sidebar_c::images_filenames;
	vec_filenames& images_filenames_hover = Config::Sidebar_c::images_filenames_hover;

	const pair_key_filename project = std::make_pair("project", reader.Get("sidebar_images", "project", "???"));
	const pair_key_filename run = std::make_pair("run", reader.Get("sidebar_images", "run", "???"));
	const pair_key_filename settings = std::make_pair("settings", reader.Get("sidebar_images", "settings", "???"));
	images_filenames.push_back(project);
	images_filenames.push_back(run);
	images_filenames.push_back(settings);

	const pair_key_filename project_hover = std::make_pair("project_hover", reader.Get("sidebar_images", "project_hover", "???"));
	const pair_key_filename run_hover = std::make_pair("run_hover", reader.Get("sidebar_images", "run_hover", "???"));
	const pair_key_filename settings_hover = std::make_pair("settings_hover", reader.Get("sidebar_images", "settings_hover", "???"));
	images_filenames_hover.push_back(project_hover);
	images_filenames_hover.push_back(run_hover);
	images_filenames_hover.push_back(settings_hover);

	const int x = reader.GetInteger("sidebar", "pos_x", -1);
	const int y = reader.GetInteger("sidebar", "pos_y", -1);
	const int w = reader.GetInteger("sidebar", "width", -1);
	const int h = reader.GetInteger("sidebar", "height", -1);
	const int px = reader.GetInteger("sidebar", "pad_x", -1);
	const int py = reader.GetInteger("sidebar", "pad_y", -1);
	const int isx = reader.GetInteger("sidebar", "item_spacing_x", -1);
	const int isy = reader.GetInteger("sidebar", "item_spacing_y", -1);
	const int max_img_width = reader.GetInteger("sidebar", "max_img_width", -1);
	const int max_img_height = reader.GetInteger("sidebar", "max_img_height", -1);

	Config::Sidebar_c::pos = ImVec2(x, y);
	Config::Sidebar_c::size = ImVec2(w, h);
	Config::Sidebar_c::max_img_size = ImVec2(max_img_width, max_img_height);
	Config::Sidebar_c::padding = ImVec2(px, py);
	Config::Sidebar_c::item_spacing = ImVec2(isx, isy);


	//sidebar indicator
	const std::string indicator_filename = reader.Get("sidebar_indicator", "indicator", "???");
	const int siw = reader.GetInteger("sidebar_indicator", "width", -1);
	const int sih = reader.GetInteger("sidebar_indicator", "height", -1);

	Config::Sidebar_c::indicator_filename = indicator_filename;
	Config::Sidebar_c::indicator_size = ImVec2(siw, sih);
}
}
