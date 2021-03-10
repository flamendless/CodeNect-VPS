#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "imgui.h"
#include "IconsFontAwesome5.h"

namespace CodeNect
{
struct Settings
{
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
	bool m_is_open = false;
	const char* m_title = ICON_FA_COG " SETTINGS";

	void init(void);
	void draw(void);
	void draw_buttons(void);
	void draw_theme_select(void);
	void draw_font_select(void);
};
}

#endif //SETTINGS_HPP
