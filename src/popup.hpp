#ifndef POPUP_H
#define POPUP_H

#include "imgui.h"

namespace CodeNect
{
struct Popup
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse;
	bool is_open = false;
};
}

#endif //POPUP_H
