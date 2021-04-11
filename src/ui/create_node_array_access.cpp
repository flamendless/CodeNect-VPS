#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"
#include "node/node_array_access.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_array_access(void)
{
	TempGetData* tmp = std::get<TempGetData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeArrayAccess* node_arr_access = static_cast<NodeArrayAccess*>(CreateNode::node_to_edit);
		node_arr_access->m_index = tmp->index;
		node_arr_access->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeArrayAccess: " << node_arr_access->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};
		in.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		out.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		std::string str = std::string(tmp->buf_str);
		NodeArrayAccess* node = new NodeArrayAccess(tmp->index, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_array_access(void)
{
	TempGetData* tmp = std::get<TempGetData*>(data);

	if (!CreateNode::is_edit_mode)
	{
		if (ImGui::BeginCombo("Type", tmp->slot_in._to_string()))
		{
			for (NODE_SLOT slot : NODE_SLOT::_values())
			{
				if (slot == +NODE_SLOT::EMPTY)
					continue;

				ImGui::PushID(slot);
				const char* txt = slot._to_string();

				if (ImGui::Selectable(txt, tmp->slot_in._to_string() == txt))
					tmp->slot_in = slot;

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}
	}

	ImGui::InputInt("Index", &tmp->index);
	static const char* txt_help = "Array index is zero-based\n"
		"this means that the index of the first element is zero";
	Utils::help_marker(txt_help, true);

	if (tmp->index < 0)
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Array index can not be negative");

	CreateNode::can_create = tmp->slot_in != +NODE_SLOT::EMPTY &&
		tmp->index >= 0;
}
}
