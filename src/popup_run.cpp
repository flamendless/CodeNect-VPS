#include "popup_run.hpp"

namespace CodeNect
{
void PopupRun::draw()
{
	bool is_inside = false;

	ImGui::Begin("Run", &is_open, flags);
		is_inside = ImGui::IsWindowHovered();

		if (ImGui::BeginMenu("Run"))
		{
			ImGui::MenuItem("Compile");
			ImGui::MenuItem("Run");
			ImGui::MenuItem("Compile and Run");
			ImGui::EndMenu();
		}
	ImGui::End();

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !is_inside)
		is_open = false;
}
}
