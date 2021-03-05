#ifndef ABOUT_HPP
#define ABOUT_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"

namespace CodeNect
{
struct About
{
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
	bool m_is_open = false;
	const char* m_title = ICON_FA_INFO_CIRCLE " ABOUT";

	void draw();
};
}

#endif //ABOUT_HPP
