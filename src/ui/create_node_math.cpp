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

		if (CreateNode::math == +NODE_MATH::SIN ||
			CreateNode::math == +NODE_MATH::COS ||
			CreateNode::math == +NODE_MATH::TAN)
			in.push_back({tmp->slot_in_first._to_string(), tmp->slot_in_first});
		else
		{
			if (tmp->slot_in_first != tmp->slot_in_second)
				in.push_back({tmp->slot_in_first._to_string(), tmp->slot_in_first});
			in.push_back({tmp->slot_in_second._to_string(), tmp->slot_in_second});
		}

		out.push_back({tmp->slot_out._to_string(), tmp->slot_out});
		NodeMath* node = new NodeMath(CreateNode::math, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_math_opt(const char* title, NODE_SLOT& slot_to_mod)
{
	if (ImGui::BeginCombo(title, slot_to_mod._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY ||
				slot == +NODE_SLOT::BOOL ||
				slot == +NODE_SLOT::STRING)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, slot_to_mod._to_string() == txt))
				slot_to_mod = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}

void CreateNode::draw_math(void)
{
	if (CreateNode::is_edit_mode)
		return;

	TempMathData* tmp = std::get<TempMathData*>(data);

	switch(CreateNode::math)
	{
		case NODE_MATH::EMPTY: break;
		case NODE_MATH::ROOT:
		{
			CreateNode::draw_math_opt("Index Input Type", tmp->slot_in_first);
			CreateNode::draw_math_opt("Radicand Input Type", tmp->slot_in_second);
			CreateNode::draw_math_opt("Output Type", tmp->slot_out);
			tmp->valid_math = tmp->slot_in_first != +NODE_SLOT::EMPTY &&
				tmp->slot_in_second != +NODE_SLOT::EMPTY &&
				tmp->slot_out != +NODE_SLOT::EMPTY;
			break;
		}
		case NODE_MATH::POW:
		{
			CreateNode::draw_math_opt("Base Input Type", tmp->slot_in_first);
			CreateNode::draw_math_opt("Exponent Input Type", tmp->slot_in_second);
			CreateNode::draw_math_opt("Output Type", tmp->slot_out);
			tmp->valid_math = tmp->slot_in_first != +NODE_SLOT::EMPTY &&
				tmp->slot_in_second != +NODE_SLOT::EMPTY &&
				tmp->slot_out != +NODE_SLOT::EMPTY;
			break;
		}
		case NODE_MATH::SIN:
		case NODE_MATH::COS:
		case NODE_MATH::TAN:
		{
			CreateNode::draw_math_opt("Input Type", tmp->slot_in_first);
			CreateNode::draw_math_opt("Output Type", tmp->slot_out);
			tmp->valid_math = tmp->slot_in_first != +NODE_SLOT::EMPTY &&
				tmp->slot_out != +NODE_SLOT::EMPTY;
			break;
		}
	}

	CreateNode::can_create = tmp->valid_math;
}
}
