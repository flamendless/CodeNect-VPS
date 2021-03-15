#include "ui/create_node.hpp"

#include "modules/node.hpp"
#include "modules/nodes.hpp"

namespace CodeNect
{
void CreateNode::create_op_node(void)
{
	//TODO this is repetitive (?) with CreateNode::create_var_node
	// Node::v_slot_info_t&& in = {};
	// Node::v_slot_info_t&& out = {};
    //
	// in.push_back({ data::op->slot._to_string(), data::op->slot });
	// out.push_back({ data::op->slot._to_string(), data::op->slot });
    //
	// Node* node = new Node(CreateNode::kind, data::op->op._to_string(), data::op->value, std::move(in), std::move(out));
    //
	// Nodes::v_nodes.push_back(node);
	// ImNodes::AutoPositionNode(Nodes::v_nodes.back());
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
				data::op->op = op;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();
}
}
