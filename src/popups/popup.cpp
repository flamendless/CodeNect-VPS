#include "popup.hpp"

#include "config.hpp"

namespace CodeNect
{
void Popup::set_center_pos()
{
	const int w = CodeNect::Config::win_width;
	const int h = CodeNect::Config::win_height;

	m_center_pos = ImVec2((float)w/2, (float)h/2);
}

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