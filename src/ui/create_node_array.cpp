#include "ui/create_node.hpp"

#include "core/utils.hpp"
#include "core/validator.hpp"
#include "node/node.hpp"
#include "node/nodes.hpp"
#include "node/node_array.hpp"

namespace CodeNect
{
const char* txt_help = "Fixed - size can not be changed later\n"
	"Dynamic - size can be automatically increased as needed";
const char* txt_size = "This can not be updated after created";

void CreateNode::create_node_array(void)
{
	TempArrayData* tmp = std::get<TempArrayData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeArray* node_array = static_cast<NodeArray*>(CreateNode::node_to_edit);
		node_array->m_name = tmp->buf_name;
		node_array->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeArray: " << node_array->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};
		in.push_back({tmp->slot._to_string(), tmp->slot});
		out.push_back({tmp->slot._to_string(), tmp->slot});
		NodeArray* node = new NodeArray(tmp->array, tmp->slot, tmp->size,
			std::move(in), std::move(out));
		node->m_name = tmp->buf_name;
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_array()
{
	TempArrayData* tmp = std::get<TempArrayData*>(data);
	const int len_name = strlen(tmp->buf_name);

	if (len_name == 0)
		Utils::display_asterisk(true);
	else
	{
		switch (tmp->res_name)
		{
			case RES_VARNAME_CONFLICT_KEYWORD:
				Utils::display_asterisk(true, STR_CONFLICT_KEYWORD); break;
			case RES_VARNAME_ALREADY_TAKEN:
				Utils::display_asterisk(true, STR_ALREADY_TAKEN); break;
		}
	}

	if (ImGui::InputText("Array Name", tmp->buf_name, IM_ARRAYSIZE(tmp->buf_name), ImGuiInputTextFlags_CharsNoBlank))
	{
		tmp->res_name = Validator::validate_var_name(tmp->buf_name);
		tmp->valid_name = tmp->res_name == RES_VARNAME_VALID;

		if (CreateNode::is_edit_mode)
		{
			if (std::strcmp(tmp->buf_name, CreateNode::node_to_edit->m_name) == 0)
			{
				tmp->res_name = RES_VARNAME_VALID;
				tmp->valid_name = true;
			}
		}
	}

	if (!CreateNode::is_edit_mode)
	{
		if (ImGui::BeginCombo("Type", tmp->slot._to_string()))
		{
			for (NODE_SLOT slot : NODE_SLOT::_values())
			{
				if (slot == +NODE_SLOT::EMPTY)
					continue;

				ImGui::PushID(slot);
				const char* txt = slot._to_string();

				if (ImGui::Selectable(txt, tmp->slot._to_string() == txt))
					tmp->slot = slot;

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Array Type", tmp->array._to_string()))
		{
			for (NODE_ARRAY array : NODE_ARRAY::_values())
			{
				if (array == +NODE_ARRAY::EMPTY)
					continue;

				ImGui::PushID(array);
				const char* txt = array._to_string();

				if (ImGui::Selectable(txt, tmp->array._to_string() == txt))
					tmp->array = array;

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		Utils::help_marker(txt_help, true);

		if (tmp->array == +NODE_ARRAY::FIXED)
		{
			ImGui::InputInt("Array Fixed Size", &tmp->size);
			Utils::help_marker(txt_size, true);
		}
	}

	tmp->valid_array = tmp->slot != +NODE_SLOT::EMPTY;
	CreateNode::can_create = tmp->valid_name && tmp->valid_array;
}
}
