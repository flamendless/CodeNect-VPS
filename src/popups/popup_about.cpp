#include "popup.hpp"

namespace CodeNect
{
void PopupAbout::draw()
{
	const int w = CodeNect::Config::win_width;
	const int h = CodeNect::Config::win_height;
	const ImVec2 pos = ImVec2((float)w/2, (float)h/2);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopup("AboutPopup"))
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
