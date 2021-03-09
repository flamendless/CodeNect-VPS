#include "popups/popup.hpp"

#include <GLFW/glfw3.h>
#include "core/project.hpp"
#include "core/app.hpp"
#include "core/defines.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
void PopupProject::draw(void)
{
	m_is_inside = false;

	PopupProject::begin_draw();
	ImGui::Begin("Project", &m_is_open, m_flags);
		m_is_inside |= ImGui::IsWindowFocused();

		if (ImGui::MenuItem("New Project"))
			Project::init_new();
				// Alert::open(ALERT_TYPE::SUCCESS, "New Project created successfully.\nPlease open the created project file");

		if (ImGui::MenuItem("Open Project"))
		{
			if (Project::open() == RES_SUCCESS)
				m_is_open = false;
			else
				Alert::open(ALERT_TYPE::ERROR, "Failed to open/read the project file");
		}

		ImGui::MenuItem("Save Project");
		ImGui::Separator();

		if (ImGui::MenuItem("Close Project", NULL, false, Project::has_open_proj))
			Project::close();

		ImGui::Separator();

		if (ImGui::MenuItem("Exit"))
			App::quit_app();
	ImGui::End();

	PopupProject::end_draw();
}
}
