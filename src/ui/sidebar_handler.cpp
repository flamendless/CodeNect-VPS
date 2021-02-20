#include "sidebar_handler.hpp"

#include <cstdint>
#include <string>
#include "config.hpp"
#include "tweeny.h"

#define DUR_FADE 50 //the higher the smoother is the fade effect

namespace CodeNect
{
tweeny::tween<float> t_sidebar_a;
tweeny::tween<float> t_sidebar_indicator_a;

bool show_sidebar = false;

void SidebarHandler::init(Sidebar* sidebar, SidebarIndicator* sidebar_indicator)
{
	m_sidebar = sidebar;
	m_sidebar_indicator = sidebar_indicator;

	t_sidebar_a = tweeny::from(m_sidebar->m_alpha)
		.to(1.0f)
		.during(DUR_FADE);

	t_sidebar_indicator_a = tweeny::from(m_sidebar_indicator->m_alpha)
		.to(0.0f)
		.during(DUR_FADE);
}

void SidebarHandler::update(float dt)
{
	const ImVec2& m_pos = ImGui::GetMousePos();
	const int min = Sidebar_c::pos.x + Sidebar_c::indicator_size.x;
	const int dist = m_pos.x - min;

	if (dist <= 0 || m_sidebar->m_has_opened)
		show_sidebar = true;
	else
		show_sidebar = false;

	if (show_sidebar)
	{
		t_sidebar_a.forward();
		t_sidebar_indicator_a.forward();
	}
	else
	{
		t_sidebar_a.backward();
		t_sidebar_indicator_a.backward();
	}

	const float da = t_sidebar_a.step(dt);
	const float da2 = t_sidebar_indicator_a.step(dt);

	m_sidebar->m_alpha = da;
	m_sidebar_indicator->m_alpha = da2;
}
}
