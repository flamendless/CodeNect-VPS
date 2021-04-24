#ifndef _BUTTON_HPP
#define _BUTTON_HPP

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
	ImVec4 m_bg = ImVec4(0, 0, 0, 0);
	ImVec4 m_tint = ImVec4(1, 1, 1, 1);

	Button();
	Button(Image* image, Image* image_hover, bool hoverable);
	CodeNect::Image* get_active_image();
	void draw();
};
}

#endif //_BUTTON_HPP
