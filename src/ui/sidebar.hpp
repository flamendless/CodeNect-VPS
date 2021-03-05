#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

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

	int init();
	int load_images();
	void check_image_size(const CodeNect::Image& img);
	void set_style();
	void unset_style();
	void manage_popups();
	void update(float dt);
	void draw();
	void draw_sidebar();
};
}

#endif //SIDEBAR_HPP
