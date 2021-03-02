#include "popup.hpp"
#include "defines.hpp"
#include "project.hpp"

namespace CodeNect
{
void PopupNewProject::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("NewProjectPopup", &m_is_open, m_popup_flags))
	{
		Utils::center_text(m_title);
		ImGui::Text("%s", m_title);
		ImGui::Separator();

		static char temp_filename[BUF_SIZE] = "";
		static char temp_title[BUF_SIZE] = "";
		static char temp_author[BUF_SIZE] = "";

		ImGui::InputText("Filename", temp_filename, IM_ARRAYSIZE(temp_title));
		ImGui::InputText("Title", temp_title, IM_ARRAYSIZE(temp_title));
		ImGui::InputText("Author", temp_author, IM_ARRAYSIZE(temp_author));
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_SAVE " Create New Project"))
		{
			m_on_create(temp_filename, temp_title, temp_author);
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button(ICON_FA_TIMES " Cancel"))
		{
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
}
