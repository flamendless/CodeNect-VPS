#include "ui/create_node.hpp"

#include "imgui.h"
#include "modules/nodes.hpp"
#include "modules/node_var.hpp"
#include "core/utils.hpp"
#include "core/validations.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
void CreateNode::create_var_node(void)
{
	v_slot_info_t&& in = {};
	v_slot_info_t&& out = {};

	in.push_back({data::var->slot._to_string(), data::var->slot});
	out.push_back({data::var->slot._to_string(), data::var->slot});

	NodeVariable* node = new NodeVariable(data::var->buf_name, data::var->value, std::move(in), std::move(out));

	Nodes::v_nodes.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());
}

void CreateNode::draw_var(void)
{
	const int len_name = strlen(data::var->buf_name);

	if (len_name == 0)
		Utils::display_asterisk(true);
	else
	{
		switch (data::var->res_name)
		{
			case RES_VARNAME_CONFLICT_KEYWORD:
				Utils::display_asterisk(true, STR_CONFLICT_KEYWORD); break;
			case RES_VARNAME_ALREADY_TAKEN:
				Utils::display_asterisk(true, STR_ALREADY_TAKEN); break;
		}
	}

	if (ImGui::InputText("Variable Name", data::var->buf_name, IM_ARRAYSIZE(data::var->buf_name), ImGuiInputTextFlags_CharsNoBlank))
	{
		data::var->res_name = Validations::validate_var_name(data::var->buf_name);
		data::var->valid_name = data::var->res_name == RES_VARNAME_VALID;
	}

	Utils::display_asterisk(data::var->slot == +NODE_SLOT::EMPTY);

	if (ImGui::BeginCombo("Data Type", data::var->slot._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, data::var->slot._to_string() == txt))
			{
				data::var->slot = slot;
				data::var->valid_value = true;
				data::var->value.set(slot);
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	if (data::var->slot != +NODE_SLOT::EMPTY)
		Utils::display_asterisk(data::var->valid_value == false);

	switch (data::var->slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: CreateNode::draw_opt_bool(); break;
		case NODE_SLOT::INTEGER: CreateNode::draw_opt_int(); break;
		case NODE_SLOT::FLOAT: CreateNode::draw_opt_float(); break;
		case NODE_SLOT::DOUBLE: CreateNode::draw_opt_double(); break;
		case NODE_SLOT::STRING: CreateNode::draw_opt_string(); break;
	}

	// finalize
	CreateNode::can_create = data::var->valid_name && data::var->valid_value;
}

void CreateNode::draw_opt_bool(void)
{
	ImGui::Text("Boolean value:");

	if (ImGui::RadioButton("true", data::var->value.v_bool == true))
		data::var->value.set(true);

	ImGui::SameLine();

	if (ImGui::RadioButton("false", data::var->value.v_bool == false))
		data::var->value.set(false);
}

void CreateNode::draw_opt_int(void)
{
	if (ImGui::InputInt("Integer value", &CreateNode::data::var->temp_int))
		data::var->value.set(CreateNode::data::var->temp_int);
}

void CreateNode::draw_opt_float(void)
{
	if (ImGui::InputFloat("Float value", &CreateNode::data::var->temp_float))
		data::var->value.set(CreateNode::data::var->temp_float);
}

void CreateNode::draw_opt_double(void)
{
	if (ImGui::InputDouble("Double value", &CreateNode::data::var->temp_double))
		data::var->value.set(CreateNode::data::var->temp_double);
}

void CreateNode::draw_opt_string(void)
{
	if (ImGui::InputText("String value", data::var->buf_string, IM_ARRAYSIZE(data::var->buf_string)))
	{
		const int len = strlen(data::var->buf_string);

		if (len > 0)
			data::var->value.set(data::var->buf_string);
	}
}
}
