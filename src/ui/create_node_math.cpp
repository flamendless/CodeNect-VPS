#include "ui/create_node.hpp"

#include "node/node.hpp"
#include "node/nodes.hpp"
#include "node/node_math.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_math(void)
{
	TempMathData* tmp = std::get<TempMathData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeMath* node_math = static_cast<NodeMath*>(CreateNode::node_to_edit);
		node_math->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeMath: " << node_math->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		switch(CreateNode::math)
		{
			case NODE_MATH::EMPTY: break;
			case NODE_MATH::ROOT:
			{
				NODE_SLOT slot = NODE_SLOT::INTEGER;
				in.push_back({slot._to_string(), slot});
				in.push_back({tmp->root_slot_in_radicand._to_string(), tmp->root_slot_out});
				out.push_back({tmp->root_slot_out._to_string(), tmp->root_slot_out});
				break;
			}
		}

		NodeMath* node = new NodeMath(CreateNode::math, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_math(void)
{
	TempMathData* tmp = std::get<TempMathData*>(data);

	switch(CreateNode::math)
	{
		case NODE_MATH::EMPTY: break;
		case NODE_MATH::ROOT:
		{
			if (ImGui::BeginCombo("Radicand Input Type", tmp->root_slot_in_radicand._to_string()))
			{
				for (NODE_SLOT slot : NODE_SLOT::_values())
				{
					if (slot == +NODE_SLOT::EMPTY ||
						slot == +NODE_SLOT::BOOL ||
						slot == +NODE_SLOT::STRING)
						continue;

					ImGui::PushID(slot);
					const char* txt = slot._to_string();

					if (ImGui::Selectable(txt, tmp->root_slot_in_radicand._to_string() == txt))
						tmp->root_slot_in_radicand = slot;

					ImGui::PopID();
				}

				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Output Type", tmp->root_slot_out._to_string()))
			{
				for (NODE_SLOT slot : NODE_SLOT::_values())
				{
					if (slot == +NODE_SLOT::EMPTY ||
						slot == +NODE_SLOT::BOOL ||
						slot == +NODE_SLOT::STRING)
						continue;

					ImGui::PushID(slot);
					const char* txt = slot._to_string();

					if (ImGui::Selectable(txt, tmp->root_slot_out._to_string() == txt))
						tmp->root_slot_out = slot;

					ImGui::PopID();
				}

				ImGui::EndCombo();
			}

			tmp->valid_math = tmp->root_slot_in_radicand != +NODE_SLOT::EMPTY &&
				tmp->root_slot_out != +NODE_SLOT::EMPTY;
		}
	}

	CreateNode::can_create = tmp->valid_math;
}
}
