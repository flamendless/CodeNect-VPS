#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "image.hpp"

namespace CodeNect
{
struct UI_Button
{
	bool hoverable = false;
	bool is_hovered = false;
	bool is_clicked = false;
	bool is_open = false;
	Image* normal;
	Image* hovered;

	ImVec2 uv0 = ImVec2();
	ImVec2 uv1 = ImVec2(1, 1);
	ImVec4 bg = ImVec4(1, 0, 0, 1); //TODO replace this
	ImVec4 tint = ImVec4(1, 1, 1, 1);

	std::string tooltip;

	CodeNect::Image* get_active_image()
	{
		if (hoverable && (is_hovered || is_open))
			return hovered;

		return normal;
	}

	void draw()
	{
		CodeNect::Image* img = get_active_image();
		is_clicked = ImGui::ImageButton(img->get_texture(), img->size, uv0, uv1, 0, bg, tint);
		is_hovered = ImGui::IsItemHovered();

		if (hoverable && is_hovered)
			ImGui::SetTooltip("%s", tooltip.c_str());
	}
};
}

#endif //UI_BUTTON_H
