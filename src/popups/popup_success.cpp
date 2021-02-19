#include "popup.hpp"

namespace CodeNect
{
void PopupSuccess::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("SuccessPopup", &m_is_open, m_popup_flags))
	{
		ImGui::Text("%s SUCCESS!", ICON_FA_CHECK);
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
		{
			m_is_open = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
}
