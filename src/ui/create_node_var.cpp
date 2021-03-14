#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "modules/node.hpp"
#include "modules/nodes.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_var_node(void)
{
	Node::v_slot_info_t&& in = {};
	Node::v_slot_info_t&& out = {};

	in.push_back({ data.slot._to_string(), data.slot });
	out.push_back({ data.slot._to_string(), data.slot });

	Node* node = new Node(CreateNode::kind, data.name, data.value, std::move(in), std::move(out));

	Nodes::v_nodes.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());

	PLOGD << "Created Variable Node: " << data.name << ", " << data.value;
}

void CreateNode::draw_var(void)
{
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

	ImGui::Separator();

	switch (data.slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: CreateNode::draw_opt_bool(); break;
		case NODE_SLOT::INT:
		{
		}
		case NODE_SLOT::FLOAT:
		{
		}
		case NODE_SLOT::DOUBLE:
		{
		}
		case NODE_SLOT::STRING:
		{
		}
	}

	//finalize
	data.can_create = data.valid_name && data.valid_value;
}

void CreateNode::draw_opt_bool(void)
{
	ImGui::RadioButton("true", &data.value_bool, 1);
	ImGui::SameLine();
	ImGui::RadioButton("false", &data.value_bool, 0);
}
}
