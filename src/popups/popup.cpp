#include "popups/popup.hpp"

#include "core/config.hpp"

namespace CodeNect
{
void Popup::begin_draw()
{
	if (is_centered)
	{
		m_pos.x = ImGui::GetIO().DisplaySize.x * 0.5f;
		m_pos.y = ImGui::GetIO().DisplaySize.y * 0.5f;
	}
	else
		m_pos.x = m_pos.x + Sidebar_c::size.x;

	ImGui::SetNextWindowPos(m_pos);
}

void Popup::end_draw()
{
	if (!m_is_inside)
		m_is_open = false;
}
}
