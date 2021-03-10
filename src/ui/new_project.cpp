#include "ui/new_project.hpp"

#include "plog/Log.h"
#include "ui/alert.hpp"
#include "modules/filesystem.hpp"
#include "core/project.hpp"
#include "core/utils.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
void display_asterisk(bool ref)
{
	if (ref)
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "*");
		ImGui::SameLine();
	}
	else
	{
		ImGui::TextColored(ImVec4(0, 0, 0, 0), "*");
		ImGui::SameLine();
	}
}

void NewProject::draw(void)
{
    ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("NewProjectPopup", &m_is_open, m_flags))
	{
		Utils::center_text(m_title, true);
		ImGui::Separator();

		static bool can_create = false;
		static std::string filepath = "";
		static char temp_filepath[BUF_SIZE] = "";
		static char temp_title[BUF_SIZE] = "";
		static char temp_author[BUF_SIZE] = "";
		int len_filepath = strlen(temp_filepath);
		int len_title = strlen(temp_title);
		int len_author = strlen(temp_author);

		display_asterisk(len_filepath == 0);
		ImGui::InputText("Filname", temp_filepath, IM_ARRAYSIZE(temp_filepath), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		if (ImGui::SmallButton("..."))
		{
			if (Filesystem::open_filepath(filepath))
				strcpy(temp_filepath, filepath.c_str());
		}

		display_asterisk(len_title == 0);
		ImGui::InputText("Title", temp_title, IM_ARRAYSIZE(temp_title));

		display_asterisk(len_author == 0);
		ImGui::InputText("Author", temp_author, IM_ARRAYSIZE(temp_author));
		ImGui::Separator();

		can_create = len_filepath > 0 && len_title > 0 && len_author > 0;

		if (!can_create)
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (ImGui::Button(ICON_FA_SAVE " Create New Project"))
		{
			if (can_create)
			{
				int res = m_on_create(filepath.c_str(), temp_title, temp_author);

				if (res == RES_FAIL)
				{
					PLOGW << "Creation of new project failed";
					Alert::open(ALERT_TYPE::ERROR, "Failed to create new project");
				}
				else
				{
					PLOGV << "Creation of new project successful";
					Alert::open(ALERT_TYPE::SUCCESS, "New Project created successfully.\nPlease open the created project file");
				}

				m_is_open = false;
				ImGui::CloseCurrentPopup();
			}
		}

		if (!can_create)
			ImGui::PopStyleVar(1);

		if (ImGui::Button(ICON_FA_TIMES " Cancel"))
		{
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
}
