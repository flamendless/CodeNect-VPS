#ifndef SIDEBAR_INDICATOR_HPP
#define SIDEBAR_INDICATOR_HPP

#include "imgui.h"
#include "ui/button.hpp"

namespace CodeNect
{
struct SidebarIndicator
{
	CodeNect::Button m_btn;
	ImGuiWindowFlags m_flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoFocusOnAppearing;
	bool m_is_open = true;
	float m_alpha = 1.0f;

	int init();
	void set_style();
	void unset_style();
	void update(float dt);
	void draw();
};
}

#endif //SIDEBAR_INDICATOR_HPP
