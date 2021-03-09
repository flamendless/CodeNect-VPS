#ifndef CUSTOM_FONT_HPP
#define CUSTOM_FONT_HPP

#include "imgui.h"

namespace CodeNect
{
struct CustomFont
{
	ImFont* font;
	float size;

	CustomFont(float size) : size{size}
	{}
};
}

#endif //CUSTOM_FONT_HPP
