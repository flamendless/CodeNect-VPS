#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "core/utils.hpp"
#include "core/defines.hpp"
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

struct
{
	bool can_create = false;
	char name[BUF_SIZE] = "";
	bool valid_name = false;
	char value[BUF_SIZE];
	bool valid_value = false;
	NODE_SLOT slot = NODE_SLOT::EMPTY;
} data;


bool first = false;

void CreateNode::open(NODE_KIND kind)
{
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

		const int len_name = strlen(data.name);
		const int len_value = strlen(data.value);

		Utils::display_asterisk(len_name == 0);

		if (ImGui::InputText("Variable Name", data.name, IM_ARRAYSIZE(data.name)))
		{
			//validate input
			data.valid_name = true;
		}

		Utils::display_asterisk(data.slot == +NODE_SLOT::EMPTY);

		if (ImGui::BeginCombo("Data Type", data.slot._to_string()))
		{
			for (NODE_SLOT slot : NODE_SLOT::_values())
			{
				if (slot == +NODE_SLOT::EMPTY)
					continue;

				ImGui::PushID(slot);
				const char* txt = slot._to_string();

				if (ImGui::Selectable(txt, data.slot._to_string() == txt))
					data.slot = slot;

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		if (data.slot != +NODE_SLOT::EMPTY)
		{
			Utils::display_asterisk(len_value == 0);

			if (ImGui::InputText("Variable Value", data.value, IM_ARRAYSIZE(data.value)))
			{
				//validate input
				data.valid_value = true;
			}
		}

		//finalize
		data.can_create = data.valid_name && data.valid_value;

		ImGui::Separator();
		CreateNode::draw_buttons();

		ImGui::End();
	}
}

void CreateNode::draw_buttons(void)
{
	if (!data.can_create)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

	if (ImGui::Button(ICON_FA_CHECK " Create"))
	{
		if (data.can_create)
		{
			switch (CreateNode::kind)
			{
				case NODE_KIND::EMPTY: break;
				case NODE_KIND::VARIABLE: CreateNode::create_var_node(); break;
			}

			CreateNode::close();
		}
	}

	if (!data.can_create)
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

void CreateNode::create_var_node(void)
{
	Node::v_slot_info_t&& in = {};
	Node::v_slot_info_t&& out = {};

	in.push_back({ data.slot._to_string(), data.slot });
	out.push_back({ data.slot._to_string(), data.slot });

	Node* node = new Node(CreateNode::kind, data.name, data.value, std::move(in), std::move(out));

	Nodes::v_nodes.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());
}
}
