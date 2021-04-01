#include "ui/create_node.hpp"

#include "node/node.hpp"
#include "node/nodes.hpp"
#include "node/node_op.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_op(void)
{
	TempOperationData* op = std::get<TempOperationData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeOperation* node_op = static_cast<NodeOperation*>(CreateNode::node_to_edit);
		node_op->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeOperation: " << node_op->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		in.push_back({op->slot._to_string(), op->slot});
		out.push_back({op->slot._to_string(), op->slot});

		NodeOperation* node = new NodeOperation(op->op, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_op(void)
{
	TempOperationData* data_op = std::get<TempOperationData*>(data);

	if (ImGui::BeginCombo("Operation Method", data_op->op._to_string()))
	{
		for (NODE_OP op : NODE_OP::_values())
		{
			if (op == +NODE_OP::EMPTY)
				continue;

			ImGui::PushID(op);
			const char* txt = op._to_string();

			if (ImGui::Selectable(txt, data_op->op._to_string() == txt))
			{
				data_op->op = op;
				data_op->valid_op = true;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	if (!data_op->valid_op)
		return;

	if (!CreateNode::is_edit_mode)
	{
		//if input slot is integer then MOD is allowed
		if (data_op->op == +NODE_OP::MOD)
		{
			data_op->slot = NODE_SLOT::INTEGER;
			Utils::help_marker("Modulo operator only allows INTEGERS", true);
		}
		else
			CreateNode::draw_opt_input();
	}

	CreateNode::can_create = data_op->slot != +NODE_SLOT::EMPTY;
}

void CreateNode::draw_opt_input(void)
{
	TempOperationData* op = std::get<TempOperationData*>(data);

	if (ImGui::BeginCombo("Type", op->slot._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY || slot == +NODE_SLOT::BOOL)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, op->slot._to_string() == txt))
				op->slot = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}
}
