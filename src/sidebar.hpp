#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <vector>
#include "imgui.h"
#include "plog/Log.h"
#include "ppk_assert.h"
#include "filesystem.hpp"
#include "config.hpp"
#include "defines.hpp"
#include "image.hpp"
#include "ui_button.hpp"
#include "popup_project.hpp"
#include "popup_run.hpp"

namespace CodeNect
{
struct Sidebar
{
	// static map_images images;
	// static map_images images_hover;
	map_ui_buttons m_ui_buttons;
	map_tooltips m_tooltips {
		{"project", "Project/File"},
		{"run", "Run"},
		{"settings", "Settings"},
	};
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoFocusOnAppearing;
	bool m_is_open = true;
	bool m_has_clicked = false;
	float m_alpha = 0.0f;

	int init();
	int load_images();
	void check_image_size(const CodeNect::Image& img);
	void set_style();
	void unset_style();
	void update(float dt);
	void draw();
	void draw_sidebar();
};
}

#endif //SIDEBAR_H
