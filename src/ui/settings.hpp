#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "imgui.h"

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

	void init();
	void draw();
	void draw_buttons();
	void draw_theme_select();
	void draw_font_select();
};
}

#endif //SETTINGS_HPP
