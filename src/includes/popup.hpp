#ifndef POPUP_HPP
#define POPUP_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "plog//Log.h"
#include "utils.hpp"

enum ALERT_TYPE
{
	SUCCESS,
	ERROR,
};

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

struct PopupAlert : Popup
{
	ALERT_TYPE m_type;
	std::string m_message;

	void open(ALERT_TYPE type, std::string msg);
	void draw();
};

struct PopupProject : Popup
{
	PopupAlert m_popup_alert;

	void init();
	void draw();
};

struct PopupRun : Popup
{
	void draw();
};

struct PopupSettings : Popup
{
	PopupAlert m_popup_alert;

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
}

#endif //POPUP_HPP
