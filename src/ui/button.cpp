#include "ui/button.hpp"
namespace CodeNect
{
Button::Button()
{
	ImGuiStyle& style = ImGui::GetStyle();
	m_bg = style.Colors[ImGuiCol_WindowBg];
	m_tint = ImVec4(1, 1, 1, 1);
}

Button::Button(Image* image, Image* image_hover, bool hoverable)
	: m_normal{image}, m_hovered{image_hover}, m_hoverable{hoverable}
{
	ImGuiStyle& style = ImGui::GetStyle();
	m_bg = style.Colors[ImGuiCol_WindowBg];
	m_tint = ImVec4(1, 1, 1, 1);
}

CodeNect::Image* Button::get_active_image()
{
	if (m_hoverable && (m_is_hovered || m_is_open))
		return m_hovered;

	return m_normal;
}

void Button::draw()
{
	CodeNect::Image* img = get_active_image();
	m_is_clicked = ImGui::ImageButton(img->get_texture(), img->size, m_uv0, m_uv1, 0, m_bg, m_tint);
	m_is_hovered = ImGui::IsItemHovered();

	if (m_has_tooltip && ImGui::IsItemHovered())
		ImGui::SetTooltip("%s", m_tooltip);
}
}
