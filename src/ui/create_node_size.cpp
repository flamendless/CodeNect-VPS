#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"
#include "node/node_size.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_size(void)
{
	TempGetData* tmp = std::get<TempGetData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeSize* node_size = static_cast<NodeSize*>(CreateNode::node_to_edit);
		node_size->m_size = tmp->size;
		node_size->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeSize: " << node_size->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};
		NODE_SLOT out_slot = NODE_SLOT::INTEGER;
		in.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		out.push_back({out_slot._to_string(), out_slot});
		NodeSize* node = new NodeSize(std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_size(void)
{
	TempGetData* tmp = std::get<TempGetData*>(data);

	if (!CreateNode::is_edit_mode)
	{
		if (ImGui::BeginCombo("Input Type", tmp->slot_in._to_string()))
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

	tmp->valid_get = tmp->slot_in != +NODE_SLOT::EMPTY;
	CreateNode::can_create = tmp->valid_get;
}
}
