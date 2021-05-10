#include "ui/create_node.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"
#include "node/node_prompt.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_prompt(void)
{
	TempActionData* tmp = std::get<TempActionData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodePrompt* node_prompt = static_cast<NodePrompt*>(CreateNode::node_to_edit);
		node_prompt->m_orig_str = tmp->buf_str;
		node_prompt->m_fake_input.set((std::string)tmp->fake_input);
		node_prompt->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodePrompt: " << node_prompt->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		tmp->slot_out = NODE_SLOT::STRING;

		in.push_back({tmp->slot_in._to_string(), tmp->slot_in});
		out.push_back({tmp->slot_out._to_string(), tmp->slot_out});

		std::string str = std::string(tmp->buf_str);
		NodePrompt* node = new NodePrompt(str, std::move(in), std::move(out));
		node->m_override = tmp->is_override;
		node->m_append_newline = tmp->is_append_newline;
		node->m_fake_input.set((std::string)tmp->fake_input);
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_prompt(void)
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
	}

	ImGui::InputText("Prompt", tmp->buf_str, IM_ARRAYSIZE(tmp->buf_str));
	if (ImGui::InputText("Fake Input", tmp->buf_fake_input, IM_ARRAYSIZE(tmp->buf_fake_input)))
		tmp->fake_input = tmp->buf_fake_input;
	Utils::help_marker("Fake user input", true);

	ImGui::Checkbox("Override", &tmp->is_override);
	Utils::help_marker("Should the prompt message be overriden by the input slot", true);
	ImGui::Checkbox("Append New Line", &tmp->is_append_newline);
	Utils::help_marker("Should the output string be appended with newline/nextline", true);

	CreateNode::can_create = tmp->slot_in != +NODE_SLOT::EMPTY;
}
}
