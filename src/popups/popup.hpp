#ifndef POPUP_HPP
#define POPUP_HPP

#include <functional>
#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "plog//Log.h"
#include "core/utils.hpp"

namespace CodeNect
{
struct Popup
{
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse;

	ImGuiWindowFlags m_popup_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;

	bool is_centered = false;
	bool m_is_open = false;
	bool m_is_inside = false;
	ImVec2 m_pos;

	void begin_draw(void);
	void end_draw(void);
};

struct PopupProject : Popup
{
	void draw(void);
};

struct PopupRun : Popup
{
	void draw(void);
};
}

#endif //POPUP_HPP
