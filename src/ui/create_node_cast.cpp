#include "ui/create_node.hpp"

#include "node/nodes.hpp"
#include "node/node_cast.hpp"

namespace CodeNect
{
void CreateNode::create_cast_node(void)
{
	if (CreateNode::is_edit_mode)
	{
		TempCastData* cast = std::get<TempCastData*>(data);
		NodeCast* node_cast = static_cast<NodeCast*>(CreateNode::node_to_edit);

		node_cast->m_desc = cast->buf_desc;

		PLOGD << "Edited NodeCast: " << node_cast->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		TempCastData* cast = std::get<TempCastData*>(data);

		in.push_back({cast->slot_in._to_string(), cast->slot_in});
		out.push_back({cast->slot_out._to_string(), cast->slot_out});

		NodeCast* node = new NodeCast(std::move(in), std::move(out));
		node->m_desc = cast->buf_desc;

		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_cast(void)
{
	TempCastData* cast = std::get<TempCastData*>(data);

	if (ImGui::BeginCombo("Input Type", cast->slot_in._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY || slot == +NODE_SLOT::STRING)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, cast->slot_in._to_string() == txt))
				cast->slot_in = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	if (cast->slot_in == +NODE_SLOT::EMPTY)
		return;

	if (ImGui::BeginCombo("Output Type", cast->slot_out._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY)
				continue;

			NODE_SLOT* slot_in = &cast->slot_in;

			//exclude certain output slot type based on the input slot type
			if (*slot_in == slot)
				continue;

			if (*slot_in == +NODE_SLOT::BOOL)
			{
				if (slot == +NODE_SLOT::FLOAT || slot == +NODE_SLOT::DOUBLE)
					continue;
			}
			else if (*slot_in == +NODE_SLOT::FLOAT || *slot_in == +NODE_SLOT::DOUBLE)
			{
				if (slot == +NODE_SLOT::BOOL)
					continue;
			}

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, cast->slot_out._to_string() == txt))
				cast->slot_out = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	CreateNode::can_create = cast->slot_in != +NODE_SLOT::EMPTY &&
		cast->slot_out != +NODE_SLOT::EMPTY;
}
}
