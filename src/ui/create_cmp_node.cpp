#include "ui/create_node.hpp"

#include "node/nodes.hpp"
#include "node/node_cmp.hpp"

namespace CodeNect
{
void CreateNode::create_cmp_node(void)
{
	if (CreateNode::is_edit_mode)
	{
		TempComparisonData* tmp = std::get<TempComparisonData*>(data);
		NodeComparison* node_cmp = static_cast<NodeComparison*>(CreateNode::node_to_edit);

		node_cmp->m_desc = tmp->buf_desc;

		PLOGD << "Edited NodeComparison: " << node_cmp->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		TempComparisonData* tmp = std::get<TempComparisonData*>(data);
		NODE_SLOT slot_out = NODE_SLOT::BOOL;

		in.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		out.push_back({slot_out._to_string(), slot_out});

		NodeComparison* node = new NodeComparison(tmp->cmp, std::move(in), std::move(out));
		node->m_desc = tmp->buf_desc;

		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_cmp(void)
{
	TempComparisonData* tmp = std::get<TempComparisonData*>(data);

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

	if (tmp->slot_in == +NODE_SLOT::EMPTY)
		return;

	if (ImGui::BeginCombo("Comparison", tmp->str.c_str()))
	{
		for (NODE_CMP cmp : NODE_CMP::_values())
		{
			if (cmp == +NODE_CMP::EMPTY)
				continue;

			//if input slot type is string, it can not be compared with:
			//GT, LT, GTE, LTE
			if (tmp->slot_in == +NODE_SLOT::STRING)
			{
				if (!(cmp == +NODE_CMP::EQ || cmp == +NODE_CMP::NEQ))
					continue;
			}

			ImGui::PushID(cmp);
			const std::pair<const char*, const char*> info = m_cmp_str[cmp._to_string()];
			std::string str = std::string(info.first) + " " + std::string(info.second);
			const char* txt = str.c_str();

			if (ImGui::Selectable(txt, tmp->cmp._to_string() == txt))
			{
				tmp->cmp = cmp;
				tmp->str = txt;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	CreateNode::can_create = tmp->slot_in != +NODE_SLOT::EMPTY &&
		tmp->cmp != +NODE_CMP::EMPTY;
}
}
