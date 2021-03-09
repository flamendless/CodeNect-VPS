#include "sidebar_handler.hpp"

#include <cstdint>
#include <string>
#include "plog/Log.h"
#include "tweeny.h"
#include "core/defines.hpp"
#include "core/config.hpp"

#define DUR_FADE_IN 1000
#define DUR_FADE_OUT 50000

namespace CodeNect
{
tweeny::tween<float> t_sidebar;
tweeny::tween<float> t_si;

bool show_sidebar = false;

void SidebarHandler::init(Sidebar* sidebar, SidebarIndicator* sidebar_indicator)
{
	m_sidebar = sidebar;
	m_sidebar_indicator = sidebar_indicator;

	t_sidebar = tweeny::from(m_sidebar->m_alpha).to(1.0f);
	t_si = tweeny::from(m_sidebar_indicator->m_alpha).to(0.0f);
}

void SidebarHandler::update(float dt)
{
	dt *= 1000;
	const ImVec2& mouse_pos = ImGui::GetMousePos();
	const int min = Sidebar_c::pos.x + Sidebar_c::indicator_size.x;
	const int dist = mouse_pos.x - min;

	if (dist <= 0 || m_sidebar->m_has_opened)
		show_sidebar = true;
	else
		show_sidebar = false;

	if (show_sidebar)
	{
		t_sidebar.during(DUR_FADE_IN);
		t_si.during(DUR_FADE_IN);

		t_sidebar.forward();
		t_si.forward();
	}
	else
	{
		t_sidebar.during(DUR_FADE_OUT);
		t_si.during(DUR_FADE_OUT);

		t_sidebar.backward();
		t_si.backward();
	}

	const float da = t_sidebar.step(dt);
	const float da2 = t_si.step(dt);

	m_sidebar->m_alpha = da;
	m_sidebar_indicator->m_alpha = da2;
}
}
