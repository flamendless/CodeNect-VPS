#include "popup.hpp"

#include "config.hpp"
#include "font.hpp"
#include <string>

namespace CodeNect
{
PopupSuccess popup_success;
int m_style_idx_orig = -1;
int m_font_size_orig = -1;

void PopupSettings::init()
{
	if (Config::style == "dark")
		m_style_idx = 0;
	else if (Config::style == "light")
		m_style_idx = 1;
	else if (Config::style == "classic")
		m_style_idx = 2;

	m_style_idx_orig = m_style_idx;
	m_font_size = Config::font_size;
	m_font_size_orig = m_font_size;

	PopupSettings::set_center_pos();
	popup_success.set_center_pos();
	popup_success.m_message = "Please restart to see the changes.";
}

void PopupSettings::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SettingsPopup", &m_is_open, m_popup_flags))
	{
		ImGui::Text("%s SETTINGS", ICON_FA_COG);
		ImGui::Separator();

		ImGui::Text("%s General", ICON_FA_HOME);

		if (ImGui::Combo(ICON_FA_PALETTE " Theme", &m_style_idx, m_styles))
		{
			switch (m_style_idx)
			{
				case 0: ImGui::StyleColorsDark(); break;
				case 1: ImGui::StyleColorsLight(); break;
				case 2: ImGui::StyleColorsClassic(); break;
			}
		}

		std::string txt_font = std::to_string(m_font_size);

		if (m_font_size == DEFAULT_FONT_SIZE)
			txt_font.append(" (Default)");

		if (ImGui::BeginCombo(ICON_FA_FONT " Font Size", txt_font.c_str()))
		{
			for (int n = MIN_FONT_SIZE; n <= MAX_FONT_SIZE; n++)
			{
				ImGui::PushID(n);

				std::string txt = std::to_string(n);

				if (n == DEFAULT_FONT_SIZE)
					txt.append(" (Default)");

				if (ImGui::Selectable(txt.c_str(), n == m_font_size))
					m_font_size = n;

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		if (m_font_size != m_font_size_orig)
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "* Save and restart needed");


		ImGui::Separator();

		PopupSettings::draw_buttons();
		popup_success.draw();

		ImGui::EndPopup();
	}
}

void PopupSettings::draw_buttons()
{
	if (ImGui::Button(ICON_FA_CHECK " Save"))
	{
		m_style_idx_orig = m_style_idx;
		m_font_size_orig = m_font_size;
		Config::update_style(m_style_idx);
		Config::update_font_size(std::to_string(m_font_size));

		bool res = Config::save_user_config();

		if (res != RES_SUCCESS)
		{
			popup_success.m_is_open = true;
			ImGui::OpenPopup("SuccessPopup");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_TIMES " Close"))
	{
		m_style_idx = m_style_idx_orig;
		m_font_size = m_font_size_orig;
		m_is_open = false;
		ImGui::CloseCurrentPopup();
	}
}
}
