#include "popup.hpp"

namespace CodeNect
{
void PopupAbout::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("AboutPopup", &m_is_open, m_popup_flags))
	{
		Utils::center_text(m_title);
		ImGui::Text("%s", m_title);
		ImGui::Separator();

		ImGui::Text("%s CodeNect: Visual Programming", ICON_FA_PROJECT_DIAGRAM);
		ImGui::Text("Software for Learning");
		ImGui::Text("Fundamentals of Programming");
		ImGui::Separator();

		ImGui::Text("%s Thesis by:", ICON_FA_BOOK);
		ImGui::Indent();
			ImGui::Text("%s Brandon Blanker Lim-it", ICON_FA_USER_COG);
			ImGui::Text("%s Jaykel O. Punay", ICON_FA_USER_NINJA);
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("%s Cavite State University", ICON_FA_SCHOOL);
		ImGui::Text("%s BSIT Batch 2017-2021", ICON_FA_USERS);
		ImGui::Separator();

		ImGui::Text("%s Special Thanks to:", ICON_FA_STAR);
		ImGui::Indent();
			ImGui::Text("%s James Angelo V. Aves (Adviser)", ICON_FA_USER_CHECK);
			ImGui::Text("%s Annabelle Almares (Technical Critic)", ICON_FA_USER_EDIT);
		ImGui::Unindent();
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
		{
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
}
