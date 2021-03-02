#include "popup.hpp"

#include <GLFW/glfw3.h>
#include "app.hpp"
#include "project.hpp"
#include "defines.hpp"

namespace CodeNect
{
void PopupProject::init()
{
	m_popup_alert.set_center_pos();
}

void PopupProject::draw()
{
	m_is_inside = false;

	PopupProject::begin_draw();
	ImGui::Begin("Project", &m_is_open, m_flags);
		m_is_inside |= ImGui::IsWindowFocused();

		if (ImGui::MenuItem("New Project"))
			Project::init_new();

		if (ImGui::MenuItem("Open Project"))
		{
			if (Project::open() == RES_SUCCESS)
				m_is_open = false;
			else
				m_popup_alert.open(ALERT_TYPE::ERROR, "Failed to open/read the project file");
		}

		ImGui::MenuItem("Save Project");
		ImGui::Separator();

		if (ImGui::MenuItem("Close Project", NULL, false, Project::has_open_proj))
			Project::close();

		ImGui::Separator();

		if (ImGui::MenuItem("Exit"))
			glfwSetWindowShouldClose(App::window, GL_TRUE);
	ImGui::End();

	m_popup_alert.draw();

	PopupProject::end_draw();
}
}
