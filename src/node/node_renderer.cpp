#include "node/node_renderer.hpp"

#include "imgui.h"
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
			}
			break;
		}
		case NODE_KIND::MATH:
		{
			NodeRenderer::draw_node_math(static_cast<NodeMath*>(node));
			break;
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

	NodeRenderer::draw_connected_op(node_op);
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

	NodeRenderer::draw_connected_cmp(node_cmp);
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

	ImGui::Checkbox("Override", &node_print->m_override);
	Utils::help_marker("Should the input string be overriden by the input slot", true);
	ImGui::Checkbox("Append New Line", &node_print->m_append_newline);
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
		// bool valid_node_cast = NodeLogic::validate_node_cast(in_node, out_node);
		NodeLogic::validate_node_cast(in_node, out_node);

		if (can_add)
		{
			in_node->new_connection(new_connection);
			out_node->new_connection(new_connection);
		}
	}

	ImNodes::CanvasState* canvas = ImNodes::GetCurrentCanvas();
	canvas->Colors[ImNodes::ColConnectionActive] = NodeColors::Connection::HOVERED;
	canvas->Colors[ImNodes::ColConnection] = NodeColors::Connection::DEFAULT;

	for (const Connection& connection : node.m_connections)
	{
		if (connection.out_node != &node) continue;

		if (connection.color == COLOR_TYPE::FALSE)
			canvas->Colors[ImNodes::ColConnection] = NodeColors::Connection::FALSE;
		else if (connection.color == COLOR_TYPE::TRUE)
			canvas->Colors[ImNodes::ColConnection] = NodeColors::Connection::TRUE;

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

		canvas->Colors[ImNodes::ColConnection] = NodeColors::Connection::DEFAULT;
	}
}

void draw_connected_op(NodeOperation* node_op)
{
	if (!node_op->m_has_valid_connections)
		return;

	std::vector<Node*> v_nodes;

	for (const Connection& connection : node_op->m_connections)
	{
		Node* out_node = (Node*)connection.out_node;
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);

		if (node_var || node_math)
			v_nodes.push_back(out_node);
	}

	std::string str_var;
	std::string str_val;
	const char* op = node_op->get_op();

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
			str_var.append(op);
			str_var.append(" ");

			str_val.append(" ");
			str_val.append(op);
			str_val.append(" ");
		}
	}

	ImGui::TextColored(Config::NodeInterface_c::label_color, "Expression:");
	ImGui::Indent();
	ImGui::BulletText("%s", str_var.c_str());
	ImGui::BulletText("%s", str_val.c_str());
}

void draw_connected_cmp(NodeComparison* node_cmp)
{
	if (!node_cmp->m_has_valid_connections)
		return;

	std::vector<NodeVariable*> v_vars;

	for (const Connection& connection : node_cmp->m_connections)
	{
		Node* in_node = (Node*)connection.in_node;
		Node* out_node = (Node*)connection.out_node;
		NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(in_node);
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);

		if (node_cmp && node_var)
			v_vars.push_back(node_var);
	}

	if (v_vars.size() < 2)
		return;

	ImGui::TextColored(Config::NodeInterface_c::label_color, "Expression:");
	std::string str_var = "";
	std::string str_val = "";

	for (int i = 0; i < v_vars.size(); i++)
	{
		NodeVariable* node_var = v_vars[i];
		str_var.append(node_var->m_name);
		str_val.append(node_var->m_value.get_value_str_ex());

		if (i < v_vars.size() - 1)
		{
			str_var.append(" ");
			str_var.append(node_cmp->get_cmp_op());
			str_var.append(" ");

			str_val.append(" ");
			str_val.append(node_cmp->get_cmp_op());
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
