#include "ui/create_node.hpp"

#include "node/node_for.hpp"
#include "node/nodes.hpp"
#include "node/node_cmp.hpp"

namespace CodeNect
{
void CreateNode::create_node_for(void)
{
	TempLoopData* temp = std::get<TempLoopData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeFor* node_for = static_cast<NodeFor*>(CreateNode::node_to_edit);
		node_for->m_start_index = temp->start_index;
		node_for->m_end_index = temp->end_index;
		node_for->m_increment = temp->increment;
		node_for->m_iterator_name = temp->buf_iterator_name;
		node_for->m_cmp = temp->cmp;
		node_for->m_desc = CreateNode::buf_desc;
		node_for->m_out_slots[1] = {temp->slot_out._to_string(), temp->slot_out};
		PLOGD << "Edited NodeFor: " << node_for->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};
		NodeFor* node = new NodeFor(temp->cmp, std::move(in), std::move(out));
		node->m_start_index = temp->start_index;
		node->m_end_index = temp->end_index;
		node->m_increment = temp->increment;
		node->m_iterator_name = temp->buf_iterator_name;
		node->m_out_slots.push_back({temp->slot_out._to_string(), temp->slot_out});
		node->create_str_code();
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

bool validate_cmp(TempLoopData* tmp)
{
	bool is_valid = false;

	switch (tmp->cmp)
	{
		case NODE_CMP::EMPTY: break;
		case NODE_CMP::NEQ:
		{
			if (tmp->increment > 0)
			{
				// for (int i = 0; i != 10; i++) //good
				// for (int i = 10; i != 0; i++) //inf
				if (tmp->start_index > tmp->end_index)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this will result in infinite loop");
				else
					is_valid = true;
			}
			else if (tmp->increment < 0)
			{
				// for (int i = 0; i != 10; i--) //inf
				// for (int i = 10; i != 0; i--) //good
				if (tmp->start_index < tmp->end_index)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this will result in infinite loop");
				else
					is_valid = true;
			}
			break;
		}

		case NODE_CMP::LT:
		{
			//for (int i = 0; i < 10; i++) //good
			//for (int i = 10; i < 0; i++) //not
			//for (int i = 0; i < 10; i--) //inf
			//for (int i = 10; i < 0; i--) //not
			if (tmp->increment > 0)
			{
				if (tmp->start_index < tmp->end_index)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			else if (tmp->increment < 0)
			{
				if (tmp->start_index < tmp->end_index)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			break;
		}

		case NODE_CMP::LTE:
		{
			//for (int i = 0; i <= 10; i++) //good
			//for (int i = 10; i <= 0; i++) //not
			//for (int i = 0; i <= 10; i--) //inf
			//for (int i = 10; i <= 0; i--) //not
			if (tmp->increment > 0)
			{
				if (tmp->start_index <= tmp->end_index)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			else if (tmp->increment < 0)
			{
				if (tmp->start_index <= tmp->end_index)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			break;
		}

		case NODE_CMP::GT:
		{
			//for (int i = 0; i > 10; i++) //not
			//for (int i = 10; i > 0; i++) //inf
			//for (int i = 0; i > 10; i--) //not
			//for (int i = 10; i > 0; i--) //good
			if (tmp->increment > 0)
			{
				if (tmp->start_index > tmp->end_index)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			else if (tmp->increment < 0)
			{
				if (tmp->start_index > tmp->end_index)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			break;
		}

		case NODE_CMP::GTE:
		{
			//for (int i = 0; i >= 10; i++) //not
			//for (int i = 10; i >= 0; i++) //inf
			//for (int i = 0; i >= 10; i--) //not
			//for (int i = 10; i >= 0; i--) //good
			if (tmp->increment > 0)
			{
				if (tmp->start_index >= tmp->end_index)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			else if (tmp->increment < 0)
			{
				if (tmp->start_index >= tmp->end_index)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: this iteration will not be triggered even once");
			}
			break;
		}
	}
	return is_valid;
}

void CreateNode::draw_for(void)
{
	TempLoopData* tmp = std::get<TempLoopData*>(data);

	ImGui::InputText("Iterator name", tmp->buf_iterator_name, IM_ARRAYSIZE(tmp->buf_iterator_name));
	ImGui::InputInt("Start Index", &tmp->start_index);
	ImGui::InputInt("End Index", &tmp->end_index);
	ImGui::InputInt("Increment", &tmp->increment);

	if (ImGui::BeginCombo("Comparison", tmp->str_cmp.c_str()))
	{
		for (NODE_CMP cmp : NODE_CMP::_values())
		{
			if (cmp == +NODE_CMP::EMPTY ||
				cmp == +NODE_CMP::EQ ||
				cmp == +NODE_CMP::AND ||
				cmp == +NODE_CMP::OR)
				continue;
			ImGui::PushID(cmp);
			const char* txt = NodeComparison::m_cmp_str[cmp._to_string()];
			if (ImGui::Selectable(txt, tmp->cmp._to_string() == txt))
			{
				tmp->cmp = cmp;
				tmp->str_cmp = txt;
			}
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Out type", tmp->slot_out._to_string()))
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

	if (tmp->cmp == +NODE_CMP::NEQ)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: using 'not equals' can result in infinite loop.\n\
				Not recommended");

	ImGui::Text("Note: you can set the numerical values later from other nodes");

	if (tmp->increment == 0)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Warning: a 0 increment may result in infinite loop");
		//TODO open in docs
		return;
	}

	bool is_valid = validate_cmp(tmp);

	tmp->valid_loop = (std::strlen(tmp->buf_iterator_name) != 0) && is_valid &&
		tmp->slot_out != +NODE_SLOT::EMPTY;
	CreateNode::can_create = tmp->valid_loop;
}
}
