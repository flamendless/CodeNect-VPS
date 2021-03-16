#ifndef _SIDEBAR_HANDLER_HPP
#define _SIDEBAR_HANDLER_HPP

#include "ui/sidebar.hpp"
#include "ui/sidebar_indicator.hpp"

namespace CodeNect
{
struct SidebarHandler
{
	Sidebar* m_sidebar;
	SidebarIndicator* m_sidebar_indicator;

	void init(Sidebar* sidebar, SidebarIndicator* sidebar_indicator);
	void update(float dt);
};
}

#endif //_SIDEBAR_HANDLER_HPP
