#include "popup.hpp"

namespace CodeNect
{
void PopupSettings::draw()
{
	m_is_inside = false;

	PopupSettings::begin_draw();
	ImGui::Begin("Settings", &m_is_open, m_flags);
		m_is_inside |= ImGui::IsWindowFocused();

		ImGui::MenuItem("Preferences");
	ImGui::End();

	PopupSettings::end_draw();
}
}
