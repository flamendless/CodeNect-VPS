#include "modules/node_renderer.hpp"

#include "imgui.h"
#include "core/config.hpp"
#include "core/utils.hpp"

namespace CodeNect::NodeRenderer
{
void draw_node(Node* node)
{
	NODE_KIND* kind = &node->m_kind;

	switch(*kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE: NodeRenderer::draw_node_var((NodeVariable*)node); break;
		case NODE_KIND::OPERATION: NodeRenderer::draw_node_op((NodeOperation*)node); break;
		case NODE_KIND::IF: break;
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

void draw_node_val(NodeValue* node_val)
{
	switch (node_val->m_slot)
	{
		case NODE_SLOT::EMPTY: ImGui::Text("Empty?! (this should be an error)"); break;
		case NODE_SLOT::BOOL: ImGui::Text("%s", std::get<bool>(node_val->data) ? "true" : "false"); break;
		case NODE_SLOT::INTEGER: ImGui::Text("%d", std::get<int>(node_val->data)); break;
		case NODE_SLOT::FLOAT: ImGui::Text("%f", std::get<float>(node_val->data)); break;
		case NODE_SLOT::DOUBLE: ImGui::Text("%.8lf", std::get<double>(node_val->data)); break;
		case NODE_SLOT::STRING: ImGui::Text("%s", std::get<std::string>(node_val->data).c_str()); break;
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
}

void draw_connections(Node& node)
{
	Connection new_connection;

	if (ImNodes::GetNewConnection(&new_connection.in_node, &new_connection.in_slot,
			&new_connection.out_node, &new_connection.out_slot))
	{
		((Node*) new_connection.in_node)->m_connections.push_back(new_connection);
		((Node*) new_connection.out_node)->m_connections.push_back(new_connection);
	}

	for (const Connection& connection : node.m_connections)
	{
		if (connection.out_node != &node) continue;
		if (!ImNodes::Connection(connection.in_node, connection.in_slot,
				connection.out_node, connection.out_slot))
		{
			((Node*) connection.in_node)->delete_connection(connection);
			((Node*) connection.out_node)->delete_connection(connection);
		}
	}
}

void draw_connected(Node& node)
{
	if (node.m_kind != +NODE_KIND::OPERATION)
		return;

	// ImGui::Separator();

	if (ImGui::TreeNode("Input Order"))
	{
		Utils::help_marker("This affects the result of the operation", true);

		for (const Connection& connection : node.m_connections)
		{
			Node* in_node = (Node*)connection.in_node;
			Node* out_node = (Node*)connection.out_node;
			NODE_KIND* in_kind = &in_node->m_kind;
			NODE_KIND* out_kind = &out_node->m_kind;

			bool is_in_op = *in_kind == +NODE_KIND::OPERATION;
			bool is_out_var = *out_kind == +NODE_KIND::VARIABLE;

			if (is_in_op && is_out_var)
			{
				NodeVariable* node_var = (NodeVariable*)out_node;

				if (ImGui::TreeNode(node_var->m_name))
				{
					if (ImGui::BeginTable("TableNode##ConnectedToOp", 2, ImGuiTableFlags_SizingFixedFit))
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::TextColored(Config::NodeInterface_c::label_color, "Name:");
						ImGui::TextColored(Config::NodeInterface_c::label_color, "Value:");

						ImGui::TableNextColumn();
						ImGui::Text("%s", node_var->m_name);
						NodeRenderer::draw_node_val(&node_var->m_value);

						ImGui::EndTable();
					}

					ImGui::TreePop();
				}
			}
		}

		ImGui::TreePop();
	}
}
}
