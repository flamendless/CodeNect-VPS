#include "node_interface.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <vector>
#include <map>
#include <string>
#include <imgui.h>
#include <imgui_internal.h>
#include "ImNodesEz.h"
#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"

namespace CodeNect
{
bool NodeInterface::is_open = true;
bool NodeInterface::has_open_file = false;
ImVec2 NodeInterface::pos;
ImVec2 NodeInterface::size;

ImGuiWindowFlags flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
	ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar;
ImVec2 center_pos;

const char* str = ICON_FA_PROJECT_DIAGRAM " Welcome to CodeNect";
const char* str2 = ICON_FA_ANGLE_LEFT " Hover on the left side to access the sidebar";
const char* str3 = ICON_FA_TERMINAL " Press <Ctrl+P> to access the command palette";

bool NodeInterface::init()
{
	const ImVec2 pos = NodeInterface_c::pos;
	const int w = Config::win_width - pos.x * 2;
	const int h = Config::win_height - pos.y * 2;

	NodeInterface::pos = pos;
	NodeInterface::size = ImVec2(w, h);

	center_pos = ImVec2((float)w/2, (float)h/2);

	return RES_SUCCESS;
}

void NodeInterface::draw()
{
	ImGui::SetNextWindowPos(NodeInterface::pos);
	ImGui::SetNextWindowSize(NodeInterface::size);
	ImGui::Begin("NodeInterface", &NodeInterface::is_open, flags);
		if (!has_open_file)
			NodeInterface::draw_startup();
		else
			NodeInterface::draw_main();
	ImGui::End();
}

void NodeInterface::draw_startup()
{
	Font::use_font(FONT_SIZE::LARGE);

	Utils::center_text(str, center_pos);
	ImGui::Text("%s", str);

	Utils::center_text(str2);
	ImGui::Text("%s", str2);

	Utils::center_text(str3);
	ImGui::Text("%s", str3);

	Font::unuse_font();
}

void NodeInterface::draw_main()
{
	static ImNodes::CanvasState canvas{};

	ImNodes::BeginCanvas(&canvas);

	ImNodes::EndCanvas();
}
}
