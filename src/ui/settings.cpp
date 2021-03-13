#include "settings.hpp"
#include <string>
#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "ui/alert.hpp"
#include "core/commands.hpp"
#include "ui/command_palette.hpp"
#include "core/font.hpp"

namespace CodeNect
{
struct SettingsData
{
	int style_idx = -1;
	int font_size = -1;
	std::string font;
	int style_idx_orig = -1;
	int font_size_orig = -1;
	std::string font_orig;

	ImVec2 cp_size_orig;
	ImVec2 cp_size;

	int fade_in;
	int fade_out;
	int fade_in_orig;
	int fade_out_orig;
} settings_data;

ImGuiWindowFlags Settings::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Settings::is_open = false;
const char* Settings::title = ICON_FA_COG " SETTINGS";

void Settings::init(void)
{
	if (Config::style == "dark")
		settings_data.style_idx = 0;
	else if (Config::style == "light")
		settings_data.style_idx = 1;
	else if (Config::style == "classic")
		settings_data.style_idx = 2;

	settings_data.style_idx_orig = settings_data.style_idx;
	settings_data.font_size = Config::font_size;
	settings_data.font_size_orig = settings_data.font_size;
	settings_data.font = Config::font;
	settings_data.font_orig = settings_data.font;
	settings_data.cp_size = Config::CommandPalette_c::size;
	settings_data.cp_size_orig = settings_data.cp_size;
	settings_data.fade_in = Config::Sidebar_c::fade_in;
	settings_data.fade_out = Config::Sidebar_c::fade_out;
	settings_data.fade_in_orig = settings_data.fade_in;
	settings_data.fade_out_orig = settings_data.fade_out;
}

void Settings::register_commands(void)
{
	Command* cmd = new Command("Settings", "open settings window", ICON_FA_COG);
	cmd->set_fn(Settings::open);
	cmd->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
}

void Settings::open(void)
{
	Settings::init();
	Settings::is_open = true;
	PLOGD << "Opened Settings window";
}

void Settings::draw(void)
{
	if (!Settings::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::Begin("Settings", &Settings::is_open, Settings::flags))
	{
		Utils::center_text(Settings::title, true);
		ImGui::Separator();

		Utils::center_text(ICON_FA_HOME " General", true);
		Settings::draw_theme_select();
		Settings::draw_font_select();
		ImGui::Separator();
		Settings::draw_sidebar();
		ImGui::Separator();
		Settings::draw_command_palette();

		if ((settings_data.font_size != settings_data.font_size_orig) ||
			(settings_data.font != settings_data.font_orig))
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "* Save and restart needed");

		ImGui::Separator();

		Settings::draw_buttons();

		ImGui::End();
	}
}

void Settings::draw_theme_select(void)
{
	if (ImGui::Combo(ICON_FA_PALETTE " Theme", &settings_data.style_idx, Config::styles))
	{
		switch (settings_data.style_idx)
		{
			case 0: ImGui::StyleColorsDark(); break;
			case 1: ImGui::StyleColorsLight(); break;
			case 2: ImGui::StyleColorsClassic(); break;
		}
	}
}

void Settings::draw_font_select(void)
{
	if (ImGui::BeginCombo(ICON_FA_FONT " Font Name", settings_data.font.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Config::fonts); n++)
		{
			ImGui::PushID(n);
			std::string txt = Config::fonts[n];

			if (ImGui::Selectable(txt.c_str(), settings_data.font == txt))
				settings_data.font = txt;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	std::string txt_font = std::to_string(settings_data.font_size);

	if (ImGui::BeginCombo(ICON_FA_TEXT_WIDTH " Font Size", txt_font.c_str()))
	{
		for (int n = MIN_FONT_SIZE; n <= MAX_FONT_SIZE; n++)
		{
			ImGui::PushID(n);
			std::string txt = std::to_string(n);

			if (ImGui::Selectable(txt.c_str(), n == settings_data.font_size))
				settings_data.font_size = n;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}

void Settings::draw_buttons(void)
{
	if (ImGui::Button(ICON_FA_CHECK " Save"))
	{
		if (settings_data.style_idx_orig != settings_data.style_idx)
			Config::update_style(settings_data.style_idx);

		if (settings_data.font_orig != settings_data.font)
			Config::update_font(settings_data.font);

		if (settings_data.font_size_orig != settings_data.font_size)
			Config::update_font_size(std::to_string(settings_data.font_size));

		if (settings_data.cp_size_orig.x != settings_data.cp_size.x ||
			settings_data.cp_size_orig.y != settings_data.cp_size.y)
			Config::update_command_palette(settings_data.cp_size);

		if (settings_data.fade_in != settings_data.fade_in_orig ||
			settings_data.fade_out != settings_data.fade_out_orig)
			Config::update_sidebar(settings_data.fade_in, settings_data.fade_out);

		bool res = Config::save_user_config();
		PLOGV << "Save user config: " << res;

		if (res == RES_SUCCESS)
		{
			settings_data.style_idx_orig = settings_data.style_idx;
			settings_data.font_size_orig = settings_data.font_size;
			settings_data.font_orig = settings_data.font;
			Alert::open(ALERT_TYPE::SUCCESS, "Configurations saved! Please restart to see the changes.");
		}
		else
			Alert::open(ALERT_TYPE::ERROR, "We encountered an error! Sorry about that.");
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_UNDO " Restore Defaults"))
	{
		bool res = Config::reset();
		PLOGV << "Restore defaults: " << res;

		if (res == RES_SUCCESS)
			Alert::open(ALERT_TYPE::SUCCESS, "Configurations restored to defaults! Please restart to see the changes.");
		else
			Alert::open(ALERT_TYPE::ERROR, "We encountered an error! Sorry about that.");
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_TIMES " Close"))
	{
		settings_data.style_idx = settings_data.style_idx_orig;
		settings_data.font_size = settings_data.font_size_orig;
		settings_data.font = settings_data.font_orig;
		Settings::is_open = false;
	}
}

void Settings::draw_sidebar(void)
{
	Utils::center_text(ICON_FA_ANGLE_LEFT " Sidebar", true);

	ImGui::SliderInt("fade in", &settings_data.fade_in, 0, 5000);
	Utils::help_marker("lower value = faster, higher value = slower", true);

	ImGui::SliderInt("fade out", &settings_data.fade_out, 0, 5000);
	Utils::help_marker("lower value = faster, higher value = slower", true);
}

void Settings::draw_command_palette(void)
{
	Utils::center_text(ICON_FA_TERMINAL " Command Palette", true);

	int width = settings_data.cp_size.x;
	int height = settings_data.cp_size.y;

	ImGui::SliderInt("width", &width, 32, Config::win_width);
	ImGui::SliderInt("height", &height, 32, Config::win_height);

	settings_data.cp_size.x = width;
	settings_data.cp_size.y = height;
}
}
