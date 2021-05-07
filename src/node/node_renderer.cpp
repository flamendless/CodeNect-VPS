#include "node/node_renderer.hpp"

#include "imgui.h"
#include "ppk_assert.h"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "node/node_logic.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeRenderer
{
void draw_node(Node* node)
{
	switch(node->m_kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE:
		{
			NodeRenderer::draw_node_var(static_cast<NodeVariable*>(node));
			break;
		}
		case NODE_KIND::OPERATION:
		{
			NodeRenderer::draw_node_op(static_cast<NodeOperation*>(node));
			break;
		}
		case NODE_KIND::CAST:
		{
			NodeRenderer::draw_node_cast(static_cast<NodeCast*>(node));
			break;
		}
		case NODE_KIND::COMPARISON:
		{
			NodeRenderer::draw_node_cmp(static_cast<NodeComparison*>(node));
			break;
		}
		case NODE_KIND::BRANCH:
		{
			NodeRenderer::draw_node_branch(static_cast<NodeBranch*>(node));
			break;
		}
		case NODE_KIND::ACTION:
		{
			NodeAction* node_action = static_cast<NodeAction*>(node);
			switch (node_action->m_action)
			{
				case NODE_ACTION::EMPTY: break;
				case NODE_ACTION::PRINT:
				{
					NodeRenderer::draw_node_print(static_cast<NodePrint*>(node));
					break;
				}
				case NODE_ACTION::PROMPT:
				{
					NodeRenderer::draw_node_prompt(static_cast<NodePrompt*>(node));
					break;
				}
			}
			break;
		}
		case NODE_KIND::MATH:
		{
			NodeRenderer::draw_node_math(static_cast<NodeMath*>(node));
			break;
		}
		case NODE_KIND::DS:
		{
			NodeDS* node_ds = static_cast<NodeDS*>(node);
			switch (node_ds->m_ds)
			{
				case NODE_DS::EMPTY: break;
				case NODE_DS::ARRAY:
				{
					NodeArray* node_array = static_cast<NodeArray*>(node);
					NodeRenderer::draw_node_array(node_array);
					break;
				}
			}
			break;
		}
		case NODE_KIND::GET:
		{
			NodeGet* node_get = static_cast<NodeGet*>(node);
			switch (node_get->m_get)
			{
				case NODE_GET::EMPTY: break;
				case NODE_GET::ARRAY_ACCESS:
				{
					NodeRenderer::draw_node_array_access(static_cast<NodeArrayAccess*>(node));
					break;
				}
				case NODE_GET::SIZE:
				{
					NodeRenderer::draw_node_size(static_cast<NodeSize*>(node));
					break;
				}
			}
			break;
		}
		case NODE_KIND::STRING:
		{
			NodeString* node_str = static_cast<NodeString*>(node);
			NodeRenderer::draw_node_string(node_str);
			break;
		}

		default:
		{
#if DEBUG_MODE
			std::string str = fmt::format("{:s} {:s} was not handled",
					node->m_name, node->m_kind._to_string());
			PPK_ASSERT(false, "%s", str.c_str());
#endif
		}
	}
}

void draw_node_val(NodeValue* node_val)
{
	switch (node_val->m_slot)
	{
		case NODE_SLOT::EMPTY: ImGui::Text("Empty?! (this should be an error)"); break;
		case NODE_SLOT::BOOL: ImGui::Text("%s", std::get<bool>(node_val->data) ? "true" : "false"); break;
		case NODE_SLOT::INTEGER: ImGui::Text("%d", std::get<int>(node_val->data)); break;
		case NODE_SLOT::FLOAT: ImGui::Text("%f", std::get<float>(node_val->data)); break;
		case NODE_SLOT::DOUBLE: ImGui::Text("%.8lf", std::get<double>(node_val->data)); break;
		case NODE_SLOT::STRING: ImGui::Text("\"%s\"", std::get<std::string>(node_val->data).c_str()); break;
	}
}

void draw_node_var(NodeVariable* node_var)
{
	if (ImGui::BeginTable("TableNode##NodeVar", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Name:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Type:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Value:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_var->m_name);
		ImGui::Text("%s", node_var->m_value_orig.m_slot._to_string());
		NodeRenderer::draw_node_val(&node_var->m_value);
		ImGui::Text("%s", node_var->m_desc);
		ImGui::EndTable();
	}
}

void draw_node_op(NodeOperation* node_op)
{
	if (ImGui::BeginTable("TableNode##NodeOperation", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Op:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s %s", node_op->m_op._to_string(), node_op->m_icon);
		ImGui::Text("%s", node_op->m_desc);
		ImGui::EndTable();
	}

	NodeRenderer::draw_connected_values(node_op);
}

void draw_node_cast(NodeCast* node_cast)
{
	if (ImGui::BeginTable("TableNode##NodeCast", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_cast->m_desc);
		ImGui::EndTable();
	}
}

void draw_node_cmp(NodeComparison* node_cmp)
{
	if (ImGui::BeginTable("TableNode##NodeComparison", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Type:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_cmp->m_str.c_str());
		ImGui::Text("%s", node_cmp->m_desc);
		ImGui::EndTable();
	}

	NodeRenderer::draw_connected_values(node_cmp);
}

void draw_node_branch(NodeBranch* node_branch)
{
	if (ImGui::BeginTable("TableNode##NodeBranch", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_branch->m_desc);
		ImGui::EndTable();
	}
}

void draw_node_print(NodePrint* node_print)
{
	if (ImGui::BeginTable("TableNode##NodePrint", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Action:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Output:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_print->m_action._to_string());
		ImGui::Text("%s", node_print->m_str.c_str());
		ImGui::Text("%s", node_print->m_desc);
		ImGui::EndTable();
	}

	ImGui::Checkbox("Append", &node_print->m_append);
	Utils::help_marker("Should the input string be appended to the original string", true);
	ImGui::Checkbox("Override", &node_print->m_override);
	Utils::help_marker("Should the input string be overriden by the input slot", true);
	ImGui::Checkbox("Append New Line", &node_print->m_append_newline);
	Utils::help_marker("Should the output string be appended with newline/nextline", true);
}

void draw_node_prompt(NodePrompt* node_prompt)
{
	if (ImGui::BeginTable("TableNode##NodePrompt", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Action:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Prompt:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_prompt->m_action._to_string());
		ImGui::Text("%s", node_prompt->m_str.c_str());
		ImGui::Text("%s", node_prompt->m_desc);
		ImGui::EndTable();
	}

	ImGui::Checkbox("Override", &node_prompt->m_override);
	Utils::help_marker("Should the prompt message be overriden by the input slot", true);
	ImGui::Checkbox("Append New Line", &node_prompt->m_append_newline);
	Utils::help_marker("Should the output string be appended with newline/nextline", true);
}

void draw_node_math(NodeMath* node_math)
{
	if (ImGui::BeginTable("TableNode##NodeMath", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Type:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "%s", node_math->get_string_a());

		if (node_math->m_needs_two_val)
			ImGui::TextColored(Config::NodeInterface_c::label_color, "%s", node_math->get_string_b());

		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_math->m_math._to_string());
		Utils::help_marker(NodeMath::m_tooltips[node_math->m_math._to_string()], true);
		node_math->draw_first();

		if (node_math->m_needs_two_val)
			node_math->draw_second();

		ImGui::Text("%s", node_math->m_desc);
		ImGui::EndTable();
	}

	if (!node_math->m_needs_two_val)
		return;

	std::vector<NodeVariable*> v_vars;
	for (const Connection& connection : node_math->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		Node* out_node = static_cast<Node*>(connection.out_node);
		NodeMath* node_math_in = dynamic_cast<NodeMath*>(in_node);
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);

		if (node_math_in && node_var)
			v_vars.push_back(node_var);
	}

	if (v_vars.size() < 2)
		return;

	std::string str = std::to_string(std::get<int>(node_math->m_first));
	str.append(" ");
	str.append(node_math->get_icon());
	str.append(" ");

	switch (node_math->m_in_slots[node_math->m_in_slots.size() - 1].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: str.append(std::to_string(std::get<int>(node_math->m_second))); break;
		case NODE_SLOT::FLOAT: str.append(std::to_string(std::get<float>(node_math->m_second))); break;
		case NODE_SLOT::DOUBLE: str.append(std::to_string(std::get<double>(node_math->m_second))); break;
		case NODE_SLOT::STRING: break;
	}

	ImGui::TextColored(Config::NodeInterface_c::label_color, "Expression:");
	ImGui::BulletText("%s", str.c_str());
}

void draw_node_array(NodeArray* node_array)
{
	const int size_a = node_array->m_elements.size();
	const int size_b = node_array->m_other_elements.size();

	if (ImGui::BeginTable("TableNode##NodeArray", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Name:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Type:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Size:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_array->m_name);
		ImGui::Text("%s", node_array->m_array._to_string());
		if (node_array->m_array == +NODE_ARRAY::FIXED)
			ImGui::Text("%d", node_array->m_fixed_size);
		else
			ImGui::Text("%d", size_a + size_b);
		ImGui::Text("%s", node_array->m_desc);
		ImGui::EndTable();
	}

	std::string str = "[";
	unsigned int i = 0;
	for (std::vector<NodeValue*>::iterator it = node_array->m_elements.begin();
		it != node_array->m_elements.end();
		it++)
	{
		NodeValue* val = *it;
		str.append(val->get_value_str_ex());
		if (i < node_array->m_elements.size() - 1)
			str.append(", ");
		if (i != 0 && i % 6 == 0)
			str.append("\n");
		i++;
	}

	i = 0;
	for (std::vector<NodeValue*>::iterator it = node_array->m_other_elements.begin();
		it != node_array->m_other_elements.end();
		it++)
	{
		NodeValue* val = *it;
		if (i == 0 && size_a != 0)
			str.append(", ");
		str.append(val->get_value_str_ex());
		if (i < node_array->m_other_elements.size() - 1)
			str.append(", ");
		if (i != 0 && i % 6 == 0)
			str.append("\n");
		i++;
	}

	i = 0;
	for (std::vector<NodeValue*>::iterator it = node_array->m_from_string.begin();
		it != node_array->m_from_string.end();
		it++)
	{
		NodeValue* val = *it;
		if (i == 0 && size_a != 0)
			str.append(", ");
		str.append(val->get_value_str_ex());
		if (i < node_array->m_from_string.size() - 1)
			str.append(", ");
		if (i != 0 && i % 6 == 0)
			str.append("\n");
		i++;
	}

	str.append("]");
	ImGui::TextColored(Config::NodeInterface_c::label_color, "Elements:");
	ImGui::BulletText("%s", str.c_str());
}

void draw_node_array_access(NodeArrayAccess* node_arr_access)
{
	if (ImGui::BeginTable("TableNode##NodeArrayAccess", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Get:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Index:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Element:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_arr_access->m_get._to_string());
		ImGui::Text("%d", node_arr_access->m_index);
		if (node_arr_access->m_current_val)
			ImGui::Text("%s", node_arr_access->m_current_val->get_value_str_ex().c_str());
		else if (!node_arr_access->m_has_array)
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "(no array input)");
		else if (node_arr_access->m_connections.size() > 0)
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "(out of bounds)");
		else
			ImGui::Text("");
		ImGui::Text("%s", node_arr_access->m_desc);
		ImGui::EndTable();
	}
}

void draw_node_size(NodeSize* node_size)
{
	if (ImGui::BeginTable("TableNode##NodeSize", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Get:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Size:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_size->m_get._to_string());
		ImGui::Text("%d", node_size->m_size);
		ImGui::Text("%s", node_size->m_desc);
		ImGui::EndTable();
	}
}

void draw_node_string(NodeString* node_str)
{
	if (ImGui::BeginTable("TableNode##NodeString", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Source:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Result:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s", node_str->m_from_str.c_str());
		ImGui::Text("%s", node_str->m_current_str.c_str());
		ImGui::Text("%s", node_str->m_desc);
		ImGui::EndTable();
	}
}

void draw_connections(Node& node)
{
	Connection new_connection;

	//query new connection
	if (ImNodes::GetNewConnection(&new_connection.in_node, &new_connection.in_slot,
			&new_connection.out_node, &new_connection.out_slot))
	{
		Node* in_node = static_cast<Node*>(new_connection.in_node);
		Node* out_node = static_cast<Node*>(new_connection.out_node);

		bool can_add = true;
		can_add &= NodeLogic::validate_node_print(in_node, out_node);
		can_add &= NodeLogic::validate_node_array(in_node, out_node);
		can_add &= NodeLogic::validate_node_string(in_node, out_node);

		if (can_add)
		{
			in_node->new_connection(new_connection);
			out_node->new_connection(new_connection);
		}
	}

	ImNodes::CanvasState* canvas = ImNodes::GetCurrentCanvas();
	canvas->Colors[ImNodes::ColConnectionActive] = NodeColors::ConnectionColors::HOVERED;
	canvas->Colors[ImNodes::ColConnection] = NodeColors::ConnectionColors::DEFAULT;

	for (const Connection& connection : node.m_connections)
	{
		if (connection.out_node != &node) continue;
		if (connection.color == COLOR_TYPE::FAIL)
			canvas->Colors[ImNodes::ColConnection] = NodeColors::ConnectionColors::FAIL;
		else if (connection.color == COLOR_TYPE::SUCCESS)
			canvas->Colors[ImNodes::ColConnection] = NodeColors::ConnectionColors::SUCCESS;
		else if (connection.color == COLOR_TYPE::RUNTIME)
			canvas->Colors[ImNodes::ColConnection] = NodeColors::ConnectionColors::RUNTIME;

		//draw connection
		//query removed connection
		if (!ImNodes::Connection(connection.in_node, connection.in_slot,
			connection.out_node, connection.out_slot))
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);

			in_node->delete_connection(connection);
			out_node->delete_connection(connection);
		}

		canvas->Colors[ImNodes::ColConnection] = NodeColors::ConnectionColors::DEFAULT;
	}
}

void draw_connected_values(Node* node)
{
	NodeOperation* node_op = dynamic_cast<NodeOperation*>(node);
	NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(node);

	if (!node_op && !node_cmp)
		return;

	if (node_op)
	{
		if (!node_op->m_has_valid_connections)
			return;
	}
	else if (node_cmp)
	{
		if (!node_cmp->m_has_valid_connections)
			return;
	}

	std::vector<Node*> v_nodes;
	for (const Connection& connection : node->m_connections)
	{
		Node* out_node = (Node*)connection.out_node;
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);

		if (node_var || node_math)
			v_nodes.push_back(out_node);
	}

	if (v_nodes.size() < 2)
		return;

	ImGui::TextColored(Config::NodeInterface_c::label_color, "Expression:");
	std::string str_var = "";
	std::string str_val = "";
	const char* sign;

	if (node_op)
		sign = node_op->get_op();
	else if (node_cmp)
		sign = node_cmp->get_cmp_op();

	for (int i = 0; i < v_nodes.size(); i++)
	{
		Node* node = v_nodes[i];
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(node);
		NodeMath* node_math = dynamic_cast<NodeMath*>(node);
		const char* name;
		NodeValue* val = nullptr;

		if (node_var)
		{
			name = node_var->m_name;
			val = &node_var->m_value;
		}
		else if (node_math)
		{
			name = node_math->m_name;
			val = node_math->m_current_val;
		}

		str_var.append(name);
		str_val.append(val->get_value_str_ex());

		if (i < v_nodes.size() - 1)
		{
			str_var.append(" ");
			str_var.append(sign);
			str_var.append(" ");

			str_val.append(" ");
			str_val.append(sign);
			str_val.append(" ");
		}
	}

	ImGui::Indent();
	ImGui::BulletText("%s", str_var.c_str());
	ImGui::BulletText("%s", str_val.c_str());
}

void push_node_style(void)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, Config::NodeInterface_c::item_inner_spacing);
}

void pop_node_style(void)
{
	ImGui::PopStyleVar(1);
}

void push_node_color(Node* node)
{
	ImNodes::CanvasState* canvas = ImNodes::GetCurrentCanvas();
	ImGui::PushStyleColor(ImGuiCol_Text, Config::NodeInterface_c::title_color);
	ImVec4* node_color = &NodeColors::m_kind[node->m_kind._to_string()];
	canvas->Colors[ImNodes::ColNodeBorder] = *node_color;
}

void pop_node_color(void)
{
	ImGui::PopStyleColor(1);
}
}
