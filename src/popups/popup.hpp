#ifndef POPUP_HPP
#define POPUP_HPP

#include <functional>
#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "plog//Log.h"
#include "utils.hpp"

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

	bool m_is_open = false;
	bool m_is_inside = false;
	ImVec2 m_center_pos;
	ImVec2 m_pos;

	void set_center_pos();
	void begin_draw();
	void end_draw();
};

struct PopupProject : Popup
{
	void init();
	void draw();
};

struct PopupRun : Popup
{
	void draw();
};
}

#endif //POPUP_HPP
