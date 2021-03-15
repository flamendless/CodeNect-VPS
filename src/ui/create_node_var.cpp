#include "imgui.h"
#include "ui/create_node.hpp"

#include "modules/node.hpp"
#include "modules/nodes.hpp"
#include "core/utils.hpp"
#include "core/validations.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
void CreateNode::create_var_node(void)
{
	Node::v_slot_info_t&& in = {};
	Node::v_slot_info_t&& out = {};

	in.push_back({ data->slot._to_string(), data->slot });
	out.push_back({ data->slot._to_string(), data->slot });

	Node* node = new Node(CreateNode::kind, data->buf_name, data->value, std::move(in), std::move(out));

	Nodes::v_nodes.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());
}

void CreateNode::draw_var(void)
{
	const int len_name = strlen(data->buf_name);

	if (len_name == 0)
		Utils::display_asterisk(true);
	else
		Utils::display_asterisk(data->res_name == RES_VARNAME_CONFLICT_KEYWORD, STR_CONFLICT_KEYWORD);

	if (ImGui::InputText("Variable Name", data->buf_name, IM_ARRAYSIZE(data->buf_name), ImGuiInputTextFlags_CharsNoBlank))
	{
		data->res_name = Validations::validate_var_name(data->buf_name);
		data->valid_name = data->res_name == RES_VARNAME_VALID;
	}

	Utils::display_asterisk(data->slot == +NODE_SLOT::EMPTY);

	if (ImGui::BeginCombo("Data Type", data->slot._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, data->slot._to_string() == txt))
			{
				data->slot = slot;
				data->valid_value = false;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	if (data->slot != +NODE_SLOT::EMPTY)
		Utils::display_asterisk(data->valid_value == false);

	switch (data->slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: CreateNode::draw_opt_bool(); break;
		case NODE_SLOT::INTEGER: CreateNode::draw_opt_int(); break;
		case NODE_SLOT::FLOAT: CreateNode::draw_opt_float(); break;
		case NODE_SLOT::DOUBLE: CreateNode::draw_opt_double(); break;
		case NODE_SLOT::STRING: CreateNode::draw_opt_string(); break;
	}

	//finalize
	data->can_create = data->valid_name && data->valid_value;
}

void CreateNode::draw_opt_bool(void)
{
	ImGui::Text("Boolean value:");

	if (ImGui::RadioButton("true", data->value.v_bool == true))
	{
		data->value.set(true);
		data->valid_value = true;
	}

	ImGui::SameLine();

	if (ImGui::RadioButton("false", data->value.v_bool == false))
	{
		data->value.set(false);
		data->valid_value = true;
	}
}

void CreateNode::draw_opt_int(void)
{
	static int temp = 0;

	if (ImGui::InputInt("Integer value", &temp))
	{
		data->value.set(temp);
		data->valid_value = true;
	}
}

void CreateNode::draw_opt_float(void)
{
	static float temp = 0;

	if (ImGui::InputFloat("Float value", &temp))
	{
		data->value.set(temp);
		data->valid_value = true;
	}
}

void CreateNode::draw_opt_double(void)
{
	static double temp = 0;

	if (ImGui::InputDouble("Double value", &temp))
	{
		data->value.set(temp);
		data->valid_value = true;
	}
}

void CreateNode::draw_opt_string(void)
{
	if (ImGui::InputText("String value", data->buf_string, IM_ARRAYSIZE(data->buf_string)))
	{
		const int len = strlen(data->buf_string);

		if (len > 0)
		{
			data->value.set(data->buf_string);
			data->valid_value = true;
		}
	}
}
}
