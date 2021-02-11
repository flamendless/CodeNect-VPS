#include "sidebar_handler.hpp"

#define MS_SIDEBAR_TOGGLE 10

namespace CodeNect
{
tweeny::tween<float> t_sidebar_a;
tweeny::tween<float> t_sidebar_indicator_a;

bool show_sidebar = false;

void SidebarHandler::init()
{
	t_sidebar_a = tweeny::from(Sidebar::alpha)
		.to(1.0f)
		.during(MS_SIDEBAR_TOGGLE);

	t_sidebar_indicator_a = tweeny::from(SidebarIndicator::alpha)
		.to(0.0f)
		.during(MS_SIDEBAR_TOGGLE);
}

void SidebarHandler::update(float dt)
{
	const ImVec2& m_pos = ImGui::GetMousePos();
	const int min = Sidebar_c::pos.x + Sidebar_c::indicator_size.x;
	const int dist = m_pos.x - min;

	if (dist <= 0)
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

	Sidebar::alpha = da;
	SidebarIndicator::alpha = da2;
}
}