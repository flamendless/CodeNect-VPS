#ifndef POPUP_HPP
#define POPUP_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "config.hpp"

namespace CodeNect
{
struct Popup
{
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse;
	bool m_is_open = false;
	bool m_is_inside = false;
	ImVec2 m_pos;

	void begin_draw();
	void end_draw();
};

struct PopupProject : Popup
{
	void draw();
};

struct PopupRun : Popup
{
	void draw();
};

struct PopupSettings : Popup
{
	void draw();
};

struct PopupAbout : Popup
{
	void draw();
};
}

#endif //POPUP_HPP
