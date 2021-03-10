#include "ui/about.hpp"
#include "core/utils.hpp"
#include "core/config.hpp"

namespace CodeNect
{
void About::draw()
{
	if (!m_is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("About", &m_is_open, m_flags))
	{
		Utils::center_text(m_title, true);
		ImGui::Separator();

		Utils::center_text(ICON_FA_PROJECT_DIAGRAM " CodeNect: Visual Programming", true);
		Utils::center_text("Software for Learning", true);
		Utils::center_text("Fundamentals of Programming", true);
		Utils::center_text(Config::version, true);
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

		ImGui::End();
	}
}
}
