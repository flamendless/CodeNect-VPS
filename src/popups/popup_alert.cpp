#include "popup.hpp"

namespace CodeNect
{
void PopupAlert::open(ALERT_TYPE type, std::string msg)
{
	m_type = type;
	m_message = msg;
	m_is_open = true;
	ImGui::OpenPopup("AlertPopup");
}

void PopupAlert::draw()
{
	ImGui::SetNextWindowPos(m_center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("AlertPopup", &m_is_open, m_popup_flags))
	{
		if (m_type == ALERT_TYPE::SUCCESS)
		{
			ImGui::Text("%s SUCCESS!", ICON_FA_CHECK);
		}
		else if (m_type == ALERT_TYPE::ERROR)
		{
			ImGui::Text("%s ERROR!", ICON_FA_BUG);
		}

		ImGui::Separator();
		ImGui::Text("%s", m_message.c_str());
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
