#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "core/utils.hpp"

namespace CodeNect
{
ImGuiWindowFlags CreateNode::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool CreateNode::is_open = false;
const char* CreateNode::title = ICON_FA_PLUS_SQUARE "Create Node";
NODE_KIND CreateNode::kind;

void CreateNode::open(NODE_KIND kind)
{
	CreateNode::kind = kind;
	CreateNode::is_open = true;
}

void CreateNode::close(void)
{
	CreateNode::is_open = false;
}

void CreateNode::draw(void)
{
	if (!CreateNode::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::Begin(CreateNode::title, &CreateNode::is_open, CreateNode::flags))
	{
		Utils::center_text(CreateNode::title, true);
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
			CreateNode::close();

		ImGui::End();
	}
}
}
