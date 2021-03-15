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

bool sidebar_is_visible = false;
bool sidebar_was_visible = false;

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

	sidebar_is_visible = (dist <= 0 || m_sidebar->m_has_opened) && !CommandPalette::is_open;

	if (sidebar_is_visible && !sidebar_was_visible)
		tween = tweeny::from(m_sidebar->m_alpha).to(1.0f).during(Config::Sidebar_c::fade_in);

	if (!sidebar_is_visible && sidebar_was_visible)
		tween = tweeny::from(m_sidebar->m_alpha).to(0.0f).during(Config::Sidebar_c::fade_out);

	sidebar_was_visible = sidebar_is_visible;

	const float value = tween.step(static_cast<int>(dt * 1000));
	m_sidebar->m_alpha = value;
}
}
