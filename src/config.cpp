#include "config.hpp"

namespace CodeNect
{
std::string Config::app_title;
int Config::win_width;
int Config::win_height;
int Config::vsync;
ImVec4 Config::clear_color;
const char* Config::config_filename = "config.ini";
std::string Config::style;

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

	CSimpleIniA reader;
	reader.SetUnicode();
	SI_Error res = reader.LoadFile(Config::config_filename);

	if (res < 0)
	{
		PLOGE << "Can't load config file: " << Config::config_filename;
		return RES_FAIL;
	}

	Config::init_general(reader);
	Config::init_sidebar(reader);

	PLOGI << "Config loaded";

	return RES_SUCCESS;
}

void Config::init_general(CSimpleIniA& reader)
{
	Config::app_title = reader.GetValue("general", "title", "CodeNect");
	Config::win_width = std::stoi(reader.GetValue("general", "width", "1024"));
	Config::win_height = std::stoi(reader.GetValue("general", "height", "720"));
	Config::vsync = std::stoi(reader.GetValue("general", "vsync", "1"));

	const float r = std::stof(reader.GetValue("clear_color", "r", "0.45f"));
	const float g = std::stof(reader.GetValue("clear_color", "g", "0.55f"));
	const float b = std::stof(reader.GetValue("clear_color", "b", "0.60f"));
	const float a = std::stof(reader.GetValue("clear_color", "a", "1.00f"));
	Config::clear_color = ImVec4(r, g, b, a);

	Config::style = reader.GetValue("general", "style", "dark");
}

void Config::init_sidebar(CSimpleIni& reader)
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
}
