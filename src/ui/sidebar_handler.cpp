#include "sidebar_handler.hpp"

#include <cstdint>
#include <string>
#include "plog/Log.h"
#include "tweeny.h"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "ui/command_palette.hpp"

namespace CodeNect
{
tweeny::tween<float> tween;

bool show_sidebar = false;

void SidebarHandler::init(Sidebar* sidebar, SidebarIndicator* sidebar_indicator)
{
	m_sidebar = sidebar;
	m_sidebar_indicator = sidebar_indicator;
}

void SidebarHandler::update(float dt)
{
	const ImVec2& mouse_pos = ImGui::GetMousePos();
	const int min = Config::Sidebar_c::pos.x + Config::Sidebar_c::indicator_size.x;
	const int dist = mouse_pos.x - min;

	if ((dist <= 0 || m_sidebar->m_has_opened) && !CommandPalette::is_open)
		show_sidebar = true;
	else
		show_sidebar = false;

	if (show_sidebar)
		tween = tweeny::from(m_sidebar->m_alpha).to(1.0f).during(Config::Sidebar_c::fade_in);
	else
		tween = tweeny::from(m_sidebar->m_alpha).to(0.0f).during(Config::Sidebar_c::fade_out);

	const float value = tween.step(static_cast<int>(dt * 1000));
	m_sidebar->m_alpha = value;
}
}
