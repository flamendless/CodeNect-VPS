#include "ui/create_node.hpp"

#include "core/utils.hpp"
#include "modules/nodes.hpp"

namespace CodeNect
{
ImGuiWindowFlags CreateNode::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
	ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse |
	ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
bool CreateNode::is_open = false;
bool CreateNode::is_pos_locked = true;
const char* CreateNode::title = ICON_FA_PLUS_SQUARE "Create Node";
NODE_KIND CreateNode::kind = NODE_KIND::EMPTY;
bool CreateNode::can_create = false;
TempVarData* CreateNode::data::var;
TempOperationData* CreateNode::data::op;

bool first = false;

void CreateNode::open(NODE_KIND kind)
{
	switch (kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE: CreateNode::data::var = new TempVarData(); break;
		case NODE_KIND::OPERATION: CreateNode::data::op = new TempOperationData(); break;
		case NODE_KIND::IF: break;
	}

	CreateNode::kind = kind;
	CreateNode::is_open = true;
	first = true;
}

void CreateNode::close(void)
{
	CreateNode::kind = NODE_KIND::EMPTY;
	CreateNode::is_open = false;
}

void CreateNode::draw(void)
{
	if (!CreateNode::is_open)
		return;

	if (first)
	{
		ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		first = false;
	}

	if (ImGui::Begin(CreateNode::title, &CreateNode::is_open, CreateNode::flags))
	{
		Utils::center_text(CreateNode::title, true);
		ImGui::Separator();

		switch (CreateNode::kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE: CreateNode::draw_var(); break;
			case NODE_KIND::OPERATION: CreateNode::draw_op(); break;
			case NODE_KIND::IF: break;
		}

		ImGui::Separator();
		CreateNode::draw_desc();
		ImGui::Separator();
		CreateNode::draw_buttons();

		ImGui::End();
	}
}

void CreateNode::draw_desc(void)
{
	char* buf = nullptr;

	switch (CreateNode::kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE: buf = data::var->buf_desc; break;
		case NODE_KIND::OPERATION: buf = data::op->buf_desc; break;
		case NODE_KIND::IF: break;
	}

	ImGui::InputText("Description", buf, BUF_SIZE * 2);
}

void CreateNode::draw_buttons(void)
{
	if (!CreateNode::can_create)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

	if (ImGui::Button(ICON_FA_CHECK " Create"))
	{
		if (CreateNode::can_create)
		{
			switch (CreateNode::kind)
			{
				case NODE_KIND::EMPTY: break;
				case NODE_KIND::VARIABLE: CreateNode::create_var_node(); break;
				case NODE_KIND::OPERATION: CreateNode::create_op_node(); break;
				case NODE_KIND::IF: break;
			}

			CreateNode::close();
		}
	}

	if (!CreateNode::can_create)
		ImGui::PopStyleVar(1);

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_TIMES " Cancel"))
		CreateNode::close();

	ImGui::SameLine();

	const char* icon = CreateNode::is_pos_locked ? ICON_FA_LOCK : ICON_FA_LOCK_OPEN;

	if (ImGui::Checkbox(icon, &CreateNode::is_pos_locked))
	{
		if (CreateNode::is_pos_locked)
			CreateNode::flags |= ImGuiWindowFlags_NoMove;
		else
			CreateNode::flags &= ~ImGuiWindowFlags_NoMove;
	}

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Lock/Unlock the position of this window");
}
}
