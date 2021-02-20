#ifndef POPUP_HPP
#define POPUP_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "plog//Log.h"

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
	void draw();
};

struct PopupRun : Popup
{
	void draw();
};

struct PopupSettings : Popup
{
	void init();
	void draw();
	void draw_buttons();
	void draw_theme_select();
	void draw_font_select();
};

struct PopupAbout : Popup
{
	void draw();
};

struct PopupSuccess : Popup
{
	std::string m_message;

	void draw();
};
}

#endif //POPUP_HPP
