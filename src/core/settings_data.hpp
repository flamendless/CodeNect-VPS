#ifndef _SETTINGS_DATA_HPP
#define _SETTINGS_DATA_HPP

#include <string>
#include "imgui.h"
#include "enum.h"

namespace CodeNect
{
BETTER_ENUM(STYLE, int, EMPTY = 0, DARK, LIGHT, CLASSIC)

struct StyleData
{
	bool changed = false;
	STYLE style = STYLE::EMPTY;
	int font_size = -1;
	std::string font;
};

struct CommandPaletteData
{
	bool changed = false;
	ImVec2 size;
};

struct SidebarData
{
	bool changed = false;
	int fade_in;
	int fade_out;
};

struct NodeInterfaceData
{
	bool changed = false;
	float label_color[3];
	float label_color_orig[3];
	float title_color[3];
	float title_color_orig[3];
	int item_inner_spacing[2];
	int item_inner_spacing_orig[2];
};
}

#endif //_SETTINGS_DATA_HPP
