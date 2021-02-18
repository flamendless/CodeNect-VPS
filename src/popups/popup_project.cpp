#include "popup.hpp"

namespace CodeNect
{
void PopupProject::draw()
{
	m_is_inside = false;

	PopupProject::begin_draw();
	ImGui::Begin("Project", &m_is_open, m_flags);
		m_is_inside |= ImGui::IsWindowFocused();

		ImGui::MenuItem("New Project");
		ImGui::MenuItem("Open Project");
		ImGui::MenuItem("Save Project");
		ImGui::Separator();
		ImGui::MenuItem("Close Project");
		ImGui::Separator();
		ImGui::MenuItem("Exit");
	ImGui::End();

	PopupProject::end_draw();
}
}
