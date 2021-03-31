#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"
#include "node/node_print.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_print(void)
{
	TempActionData* tmp = std::get<TempActionData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodePrint* node_print = static_cast<NodePrint*>(CreateNode::node_to_edit);
		node_print->m_orig_str = tmp->buf_str;
		node_print->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodePrint: " << node_print->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		in.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		out.push_back({tmp->slot_out._to_string(), tmp->slot_out});

		std::string str = std::string(tmp->buf_str);
		NodePrint* node = new NodePrint(str, std::move(in), std::move(out));
		node->m_override = tmp->is_override;
		node->m_append_newline = tmp->is_append_newline;
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_print(void)
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

	ImGui::InputText("Output", tmp->buf_str, IM_ARRAYSIZE(tmp->buf_str));

	ImGui::Checkbox("Override", &tmp->is_override);
	Utils::help_marker("Should the input string be overriden by the input slot", true);
	ImGui::Checkbox("Append New Line", &tmp->is_append_newline);
	Utils::help_marker("Should the output string be appended with newline/nextline", true);

	CreateNode::can_create = tmp->slot_in != +NODE_SLOT::EMPTY &&
		tmp->slot_out != +NODE_SLOT::EMPTY;
}
}
