#include "popup_project.hpp"

namespace CodeNect
{
void PopupProject::draw()
{
	bool is_inside = false;

	ImGui::Begin("Project", &is_open, flags);
		is_inside = ImGui::IsWindowHovered();

		if (ImGui::BeginMenu("Project"))
		{
			ImGui::MenuItem("New Project");
			ImGui::MenuItem("Open Project");
			ImGui::MenuItem("Save Project");
			ImGui::EndMenu();
		}
	ImGui::End();

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !is_inside)
		is_open = false;
}
}
