#include "popups/popup.hpp"

namespace CodeNect
{
void PopupRun::draw()
{
	m_is_inside = false;

	PopupRun::begin_draw();
	ImGui::Begin("Run", &m_is_open, m_flags);
		m_is_inside |= ImGui::IsWindowFocused();

		ImGui::MenuItem("Compile");
		ImGui::MenuItem("Run");
		ImGui::MenuItem("Compile and Run");
	ImGui::End();

	PopupRun::end_draw();
}
}
