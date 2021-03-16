#ifndef _CUSTOM_FONT_HPP
#define _CUSTOM_FONT_HPP

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

#endif //_CUSTOM_FONT_HPP
