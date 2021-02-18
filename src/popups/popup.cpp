#include "popup.hpp"

namespace CodeNect
{
void Popup::begin_draw()
{
	m_pos.x = m_pos.x + Sidebar_c::size.x;
	ImGui::SetNextWindowPos(m_pos);
}

void Popup::end_draw()
{
	if (!m_is_inside)
		m_is_open = false;
}
}
