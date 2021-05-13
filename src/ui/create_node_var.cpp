#include "ui/create_node.hpp"

#include "imgui.h"
#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "core/utils.hpp"
#include "core/validator.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
void CreateNode::create_node_var(void)
{
	TempVarData* var = std::get<TempVarData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeVariable* node_var = static_cast<NodeVariable*>(CreateNode::node_to_edit);
		node_var->m_name = var->buf_name;
		node_var->m_desc = CreateNode::buf_desc;
		node_var->m_value_orig.copy(var->value);
		PLOGD << "Edited NodeVariable: " << node_var->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		in.push_back({var->slot._to_string(), var->slot});
		out.push_back({var->slot._to_string(), var->slot});

		NodeVariable* node = new NodeVariable(var->buf_name, var->value, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_var(void)
{
	TempVarData* var = std::get<TempVarData*>(data);
	const int len_name = strlen(var->buf_name);

	if (len_name == 0)
		Utils::display_asterisk(true);
	else
	{
		switch (var->res_name)
		{
			case RES_VARNAME_CONFLICT_KEYWORD:
				Utils::display_asterisk(true, STR_CONFLICT_KEYWORD); break;
			case RES_VARNAME_ALREADY_TAKEN:
				Utils::display_asterisk(true, STR_ALREADY_TAKEN); break;
			case RES_VARNAME_INVALID:
				Utils::display_asterisk(true, STR_INVALID); break;
		}
	}

	if (ImGui::InputText("Variable Name", var->buf_name, IM_ARRAYSIZE(var->buf_name), ImGuiInputTextFlags_CharsNoBlank))
	{
		var->res_name = Validator::validate_var_name(var->buf_name);
		var->valid_name = var->res_name == RES_VARNAME_VALID;

		if (CreateNode::is_edit_mode)
		{
			if (std::strcmp(var->buf_name, CreateNode::node_to_edit->m_name) == 0)
			{
				var->res_name = RES_VARNAME_VALID;
				var->valid_name = true;
			}
		}
	}

	if (var->slot == +NODE_SLOT::EMPTY)
		Utils::display_asterisk(true);

	if (!CreateNode::is_edit_mode)
	{
		if (ImGui::BeginCombo("Data Type", var->slot._to_string()))
		{
			for (NODE_SLOT slot : NODE_SLOT::_values())
			{
				if (slot == +NODE_SLOT::EMPTY)
					continue;

				ImGui::PushID(slot);
				const char* txt = slot._to_string();

				if (ImGui::Selectable(txt, var->slot._to_string() == txt))
				{
					var->slot = slot;
					var->valid_value = true;
					var->value.copy(slot);
				}

				ImGui::PopID();
			}

			ImGui::EndCombo();
		}
	}

	ImGui::Separator();

	switch (var->slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: CreateNode::draw_opt_bool(&var->value); break;
		case NODE_SLOT::INTEGER: CreateNode::draw_opt_int(&var->value, var->temp_int); break;
		case NODE_SLOT::FLOAT: CreateNode::draw_opt_float(&var->value, var->temp_float); break;
		case NODE_SLOT::DOUBLE: CreateNode::draw_opt_double(&var->value, var->temp_double); break;
		case NODE_SLOT::STRING: CreateNode::draw_opt_string(&var->value, var->buf_string); break;
	}

	// finalize
	CreateNode::can_create = var->valid_name && var->valid_value;
}

void CreateNode::draw_opt_bool(NodeValue* val)
{
	ImGui::Text("Boolean value:");
	if (ImGui::RadioButton("true", std::get<bool>(val->data) == true))
		val->set(true);
	ImGui::SameLine();
	if (ImGui::RadioButton("false", std::get<bool>(val->data) == false))
		val->set(false);
}

void CreateNode::draw_opt_int(NodeValue* val, int& temp_int)
{
	if (ImGui::InputInt("Integer value", &temp_int))
		val->set(temp_int);
}

void CreateNode::draw_opt_float(NodeValue* val, float& temp_float)
{
	if (ImGui::InputFloat("Float value", &temp_float))
		val->set(temp_float);
}

void CreateNode::draw_opt_double(NodeValue* val, double& temp_double)
{
	if (ImGui::InputDouble("Double value", &temp_double))
		val->set(temp_double);
}

void CreateNode::draw_opt_string(NodeValue* val, char buf_string[BUF_SIZE * 2])
{
	if (ImGui::InputText("String value", buf_string, BUF_SIZE * 2))
	{
		std::string str = std::string(buf_string);
		val->set(str);
	}
}
}
