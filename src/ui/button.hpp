#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "core/image.hpp"

namespace CodeNect
{
struct Button
{
	Image* m_normal;
	Image* m_hovered;
	bool m_hoverable = false;
	bool m_is_hovered = false;
	bool m_is_clicked = false;
	bool m_is_open = false;

	ImVec2 m_uv0 = ImVec2();
	ImVec2 m_uv1 = ImVec2(1, 1);
	ImVec4 m_bg = ImVec4(1, 0, 0, 1); //TODO replace this
	ImVec4 m_tint = ImVec4(1, 1, 1, 1);

	Button(){}

	Button(Image* image, Image* image_hover, bool hoverable)
		: m_normal{image}, m_hovered{image_hover}, m_hoverable{hoverable}
	{
	}

	CodeNect::Image* get_active_image()
	{
		if (m_hoverable && (m_is_hovered || m_is_open))
			return m_hovered;

		return m_normal;
	}

	void draw()
	{
		CodeNect::Image* img = get_active_image();
		m_is_clicked = ImGui::ImageButton(img->get_texture(), img->size, m_uv0, m_uv1, 0, m_bg, m_tint);
		m_is_hovered = ImGui::IsItemHovered();
	}
};
}

#endif //BUTTON_HPP
