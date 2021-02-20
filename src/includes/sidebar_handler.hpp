#ifndef SIDEBAR_HANDLER_HPP
#define SIDEBAR_HANDLER_HPP

#include "sidebar.hpp"
#include "sidebar_indicator.hpp"

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

#endif //SIDEBAR_HANDLER_HPP
