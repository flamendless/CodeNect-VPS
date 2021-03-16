#ifndef _SIDEBAR_HPP
#define _SIDEBAR_HPP

#include "imgui.h"
#include "core/defines.hpp"

namespace CodeNect
{
struct Sidebar
{
	// static map_images images;
	// static map_images images_hover;
	map_ui_buttons m_ui_buttons;
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing;
	bool m_is_open = true;
	bool m_has_opened = false;
	float m_alpha = 0.0f;

	int init(void);
	int load_images(void);
	void check_image_size(const CodeNect::Image& img);
	void set_style(void);
	void unset_style(void);
	void manage_popups(void);
	void draw(void);
	void draw_sidebar(void);
	void shutdown(void);
};
}

#endif //_SIDEBAR_HPP
