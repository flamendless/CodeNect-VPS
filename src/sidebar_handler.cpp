#include "sidebar_handler.hpp"

namespace CodeNect
{
tweeny::tween<int> SidebarHandler::t_sidebar_x;
tweeny::tween<int> SidebarHandler::t_sidebar_indicator_x;

void SidebarHandler::init()
{
	SidebarHandler::t_sidebar_x = tweeny::from((int)Sidebar::cur_pos.x)
		.to(-Sidebar_c::size.x)
		.during(MS_SIDEBAR_TOGGLE)
		.via(tweeny::easing::backIn);

	SidebarHandler::t_sidebar_indicator_x = tweeny::from((int)SidebarIndicator::cur_pos.x)
		.to(0)
		.during(MS_SIDEBAR_TOGGLE)
		.via(tweeny::easing::backOut);
}

void SidebarHandler::update(float dt)
{
	// int dx = SidebarHandler::t_sidebar_x.step(dt);
	// SidebarIndicator::cur_pos.x = dx;
    //
	// int dx_indicator = SidebarHandler::t_sidebar_indicator_x.step(dt);
	// Sidebar::cur_pos.x = dx_indicator;
}
}
