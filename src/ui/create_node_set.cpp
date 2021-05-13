#include "ui/create_node.hpp"

#include "core/utils.hpp"
#include "node/node_set.hpp"
#include "node/nodes.hpp"

namespace CodeNect
{
void CreateNode::create_node_set(void)
{
	TempActionData* tmp = std::get<TempActionData*>(data);
	if (CreateNode::is_edit_mode)
	{
		NodeSet* node_set = static_cast<NodeSet*>(CreateNode::node_to_edit);
		node_set->m_node_var = tmp->node_var;
		node_set->m_node_val_orig.copy(tmp->node_val);
		PLOGD << "Edited NodeSet: " << node_set->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};
		in.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		out.push_back({tmp->slot_out._to_string(), tmp->slot_out});
		NodeSet* node = new NodeSet(tmp->node_var, tmp->node_val, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_set(void)
{
	TempActionData* tmp = std::get<TempActionData*>(data);
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

		if (ImGui::BeginCombo("Output Type", tmp->slot_out._to_string()))
		{
			for (NODE_SLOT slot : NODE_SLOT::_values())
			{
				if (slot == +NODE_SLOT::EMPTY)
					continue;
				ImGui::PushID(slot);
				const char* txt = slot._to_string();
				if (ImGui::Selectable(txt, tmp->slot_out._to_string() == txt))
					tmp->slot_out = slot;
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}
	}

	//select existing node_var
	const char* txt = (tmp->node_var) ? tmp->node_var->m_name : "(none selected)";
	if (ImGui::BeginCombo("Variable name", txt))
	{
		for (Node* &node : Nodes::v_nodes)
		{
			if (node->m_kind != +NODE_KIND::VARIABLE)
				continue;
			if (CreateNode::node_to_edit && CreateNode::node_to_edit == node)
				continue;
			ImGui::PushID(node);
			if (ImGui::Selectable(node->m_name, txt == node->m_name))
			{
				tmp->node_var = static_cast<NodeVariable*>(node);
				tmp->slot_val = tmp->node_var->m_value.m_slot;
				switch (tmp->slot_val)
				{
					case NODE_SLOT::EMPTY: break;
					case NODE_SLOT::BOOL: tmp->node_val.set((bool)false); break;
					case NODE_SLOT::INTEGER: tmp->node_val.set((int)0); break;
					case NODE_SLOT::FLOAT: tmp->node_val.set((float)0.0f); break;
					case NODE_SLOT::DOUBLE: tmp->node_val.set((double)0); break;
					case NODE_SLOT::STRING: tmp->node_val.set((std::string)""); break;
				}
			}
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}

	switch (tmp->slot_val)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: CreateNode::draw_opt_bool(&tmp->node_val); break;
		case NODE_SLOT::INTEGER: CreateNode::draw_opt_int(&tmp->node_val, tmp->temp_int); break;
		case NODE_SLOT::FLOAT: CreateNode::draw_opt_float(&tmp->node_val, tmp->temp_float); break;
		case NODE_SLOT::DOUBLE: CreateNode::draw_opt_double(&tmp->node_val, tmp->temp_double); break;
		case NODE_SLOT::STRING: CreateNode::draw_opt_string(&tmp->node_val, tmp->buf_string); break;
	}

	if (tmp->slot_val != +NODE_SLOT::EMPTY)
		Utils::help_marker(std::move("You can set the value from other Node"), true);

	CreateNode::can_create = tmp->slot_in != +NODE_SLOT::EMPTY &&
		tmp->slot_out != +NODE_SLOT::EMPTY &&
		tmp->node_val.m_slot != +NODE_SLOT::EMPTY &&
		tmp->node_var != nullptr;
}
}
