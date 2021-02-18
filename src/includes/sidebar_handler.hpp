#ifndef SIDEBAR_HANDLER_H
#define SIDEBAR_HANDLER_H

#include <cstdint>
#include <string>
#include "tweeny.h"
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

#endif //SIDEBAR_HANDLER_H
