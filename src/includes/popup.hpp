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
	ImGuiWindowFlags m_popup_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse;

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
	void draw();
};

struct PopupRun : Popup
{
	void draw();
};

struct PopupSettings : Popup
{
	int m_style_idx = -1;
	const char* m_styles = "Dark\0Light\0Classic\0";

	void init();
	void draw();
};

struct PopupAbout : Popup
{
	void draw();
};

struct PopupSuccess : Popup
{
	void draw();
};
}

#endif //POPUP_HPP
