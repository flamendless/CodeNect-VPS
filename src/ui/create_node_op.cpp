#include "ui/create_node.hpp"

#include "modules/node.hpp"
#include "modules/nodes.hpp"
#include "modules/node_op.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_op_node(void)
{
	v_slot_info_t&& in = {};
	v_slot_info_t&& out = {};

	in.push_back({data::op->slot._to_string(), data::op->slot});
	out.push_back({data::op->slot._to_string(), data::op->slot});

	NodeOperation* node = new NodeOperation(data::op->op, std::move(in), std::move(out));
	node->m_desc = data::op->buf_desc;

	Nodes::v_nodes.push_back(node);
	Nodes::v_nodes_op.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());
}

void CreateNode::draw_op(void)
{
	if (ImGui::BeginCombo("Operation Method", data::op->op._to_string()))
	{
		for (NODE_OP op : NODE_OP::_values())
		{
			if (op == +NODE_OP::EMPTY)
				continue;

			ImGui::PushID(op);
			const char* txt = op._to_string();

			if (ImGui::Selectable(txt, data::op->op._to_string() == txt))
			{
				data::op->op = op;
				data::op->valid_op = true;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	if (!data::op->valid_op)
		return;

	CreateNode::draw_opt_input();
	CreateNode::can_create = data::op->slot != +NODE_SLOT::EMPTY;
}

void CreateNode::draw_opt_input(void)
{
	if (ImGui::BeginCombo("Type", data::op->slot._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY || slot == +NODE_SLOT::BOOL)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, data::op->slot._to_string() == txt))
				data::op->slot = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}
}
