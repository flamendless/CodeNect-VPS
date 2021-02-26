#include "popup.hpp"

#include "config.hpp"
#include "font.hpp"
#include <string>

namespace CodeNect
{
int style_idx = -1;
int font_size = -1;
std::string font;
int style_idx_orig = -1;
int font_size_orig = -1;
std::string font_orig;

void PopupSettings::init()
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

	PopupSettings::set_center_pos();
	m_popup_alert.set_center_pos();
}

void PopupSettings::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SettingsPopup", &m_is_open, m_popup_flags))
	{
		const char* title = ICON_FA_COG " SETTINGS";
		Utils::center_text(title);

		ImGui::Text("%s", title);
		ImGui::Separator();

		ImGui::Text("%s General", ICON_FA_HOME);
		PopupSettings::draw_theme_select();
		PopupSettings::draw_font_select();

		if ((font_size != font_size_orig) || (font != font_orig))
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "* Save and restart needed");

		ImGui::Separator();

		PopupSettings::draw_buttons();
		m_popup_alert.draw();

		ImGui::EndPopup();
	}
}

void PopupSettings::draw_theme_select()
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

void PopupSettings::draw_font_select()
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

void PopupSettings::draw_buttons()
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
			m_popup_alert.open(ALERT_TYPE::SUCCESS, "Configurations saved! Please restart to see the changes.");
		}
		else
		{
			m_popup_alert.open(ALERT_TYPE::ERROR, "We encountered an error! Sorry about that.");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_UNDO " Restore Defaults"))
	{
		bool res = Config::reset();
		PLOGV << "Restore defaults: " << res;

		if (res == RES_SUCCESS)
		{
			m_popup_alert.open(ALERT_TYPE::SUCCESS, "Configurations restored to defaults! Please restart to see the changes.");
		}
		else
		{
			m_popup_alert.open(ALERT_TYPE::ERROR, "We encountered an error! Sorry about that.");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_TIMES " Close"))
	{
		style_idx = style_idx_orig;
		font_size = font_size_orig;
		font = font_orig;
		m_is_open = false;
		ImGui::CloseCurrentPopup();
	}
}
}
