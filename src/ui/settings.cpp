#include "settings.hpp"
#include <string>
#include "plog/Log.h"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
int style_idx = -1;
int font_size = -1;
std::string font;
int style_idx_orig = -1;
int font_size_orig = -1;
std::string font_orig;

void Settings::init(void)
{
	if (Config::style == "dark")
		style_idx = 0;
	else if (Config::style == "light")
		style_idx = 1;
	else if (Config::style == "classic")
		style_idx = 2;

	style_idx_orig = style_idx;
	font_size = Config::font_size;
	font_size_orig = font_size;
	font = Config::font;
	font_orig = font;
}

void Settings::draw(void)
{
	if (!m_is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("Settings", &m_is_open, m_flags))
	{
		Utils::center_text(m_title, true);
		ImGui::Separator();

		ImGui::Text("%s General", ICON_FA_HOME);
		Settings::draw_theme_select();
		Settings::draw_font_select();

		if ((font_size != font_size_orig) || (font != font_orig))
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "* Save and restart needed");

		ImGui::Separator();

		Settings::draw_buttons();

		ImGui::End();
	}
}

void Settings::draw_theme_select(void)
{
	if (ImGui::Combo(ICON_FA_PALETTE " Theme", &style_idx, Config::styles))
	{
		switch (style_idx)
		{
			case 0: ImGui::StyleColorsDark(); break;
			case 1: ImGui::StyleColorsLight(); break;
			case 2: ImGui::StyleColorsClassic(); break;
		}
	}
}

void Settings::draw_font_select(void)
{
	if (ImGui::BeginCombo(ICON_FA_FONT " Font Name", font.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Config::fonts); n++)
		{
			ImGui::PushID(n);
			std::string txt = Config::fonts[n];

			if (ImGui::Selectable(txt.c_str(), font == txt))
				font = txt;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	std::string txt_font = std::to_string(font_size);

	if (ImGui::BeginCombo(ICON_FA_TEXT_WIDTH " Font Size", txt_font.c_str()))
	{
		for (int n = MIN_FONT_SIZE; n <= MAX_FONT_SIZE; n++)
		{
			ImGui::PushID(n);
			std::string txt = std::to_string(n);

			if (ImGui::Selectable(txt.c_str(), n == font_size))
				font_size = n;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}

void Settings::draw_buttons(void)
{
	if (ImGui::Button(ICON_FA_CHECK " Save"))
	{
		if (style_idx_orig != style_idx)
			Config::update_style(style_idx);

		if (font_orig != font)
			Config::update_font(font);

		if (font_size_orig != font_size)
			Config::update_font_size(std::to_string(font_size));

		bool res = Config::save_user_config();
		PLOGV << "Save user config: " << res;

		if (res == RES_SUCCESS)
		{
			style_idx_orig = style_idx;
			font_size_orig = font_size;
			font_orig = font;
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
		style_idx = style_idx_orig;
		font_size = font_size_orig;
		font = font_orig;
		m_is_open = false;
	}
}
}
