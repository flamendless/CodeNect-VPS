#include "popup.hpp"

namespace CodeNect
{
void PopupSettings::draw()
{
	ImGui::SetNextWindowPos(m_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopup("SettingsPopup"))
	{
		ImGui::Text("%s SETTINGS", ICON_FA_COG);
		ImGui::Separator();

		if (ImGui::Combo("Theme", &m_style_idx, m_styles))
		{
			switch(m_style_idx)
			{
				case 0: ImGui::StyleColorsDark(); break;
				case 1: ImGui::StyleColorsLight(); break;
				case 2: ImGui::StyleColorsClassic(); break;
			}
		}
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_CHECK " Save"))
		{
		}

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
		{
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
}
