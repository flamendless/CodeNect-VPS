#include "popup.hpp"

#include "config.hpp"

namespace CodeNect
{
PopupSuccess popup_success;

void PopupSettings::init()
{
	if (Config::style == "dark")
		m_style_idx = 0;
	else if (Config::style == "light")
		m_style_idx = 1;
	else if (Config::style == "classic")
		m_style_idx = 2;

	m_style_idx_orig = m_style_idx;

	PopupSettings::set_center_pos();
	popup_success.set_center_pos();
}

void PopupSettings::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SettingsPopup", &m_is_open, m_popup_flags))
	{
		ImGui::Text("%s SETTINGS", ICON_FA_COG);
		ImGui::Separator();

		if (ImGui::Combo("Theme", &m_style_idx, m_styles))
		{
			switch (m_style_idx)
			{
				case 0: ImGui::StyleColorsDark(); break;
				case 1: ImGui::StyleColorsLight(); break;
				case 2: ImGui::StyleColorsClassic(); break;
			}
		}

		ImGui::Separator();

		if (ImGui::Button(ICON_FA_CHECK " Save"))
		{
			m_style_idx_orig = m_style_idx;
			Config::update_style(m_style_idx);

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
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}

		popup_success.draw();
		ImGui::EndPopup();
	}
}
}
