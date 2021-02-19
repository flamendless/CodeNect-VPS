#include "popup.hpp"

namespace CodeNect
{
void PopupSettings::draw()
{
	ImGui::SetNextWindowPos(m_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopup("SettingsPopup"))
	{
		ImGui::Text("CodeNect: Visual Programming");
		ImGui::Text("Software for Learning");
		ImGui::Text("Fundamentals of Programming");
		ImGui::Separator();

		ImGui::Text("Thesis by:");
		ImGui::Indent();
			ImGui::Text("Brandon Blanker Lim-it");
			ImGui::Text("Jaykel O. Punay");
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Cavite State University");
		ImGui::Text("BSIT Batch 2017-2021");

		if (ImGui::Button("Close"))
		{
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
}
