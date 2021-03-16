#ifndef _NEW_PROJECT_HPP
#define _NEW_PROJECT_HPP

#include <functional>
#include "imgui.h"
#include "IconsFontAwesome5.h"

namespace CodeNect
{
struct NewProject
{
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
	bool m_is_open = false;
	const char* m_title = ICON_FA_FOLDER_OPEN " NEW PROJECT";
	std::function <int(const char*, const char*, const char*)> m_on_create;

	void draw(void);
};
}

#endif //_NEW_PROJECT_HPP
