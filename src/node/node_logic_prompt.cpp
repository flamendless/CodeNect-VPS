#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_prompt.hpp"
#include "node/node_cast.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_prompt(NodePrompt* node_prompt)
{
	node_prompt->m_str = node_prompt->m_orig_str;

	//get the value of connected node_var and set to ours
	if (node_prompt->m_override)
	{
		for (const Connection& connection : node_prompt->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodePrompt* in_node_prompt = dynamic_cast<NodePrompt*>(in_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

			//check if connected is node_var
			if (in_node_prompt && out_node_var)
			{
				NodeValue* val = &out_node_var->m_value;
				node_prompt->m_str = val->get_value_str_ex();
			}
		}
	}

	//manage color
	for (Connection& connection : node_prompt->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
		NodeCast* node_cast = dynamic_cast<NodeCast*>(in_node);

		if (node_var)
			NodeColors::set_connection_color(connection, COLOR_TYPE::RUNTIME);
		if (node_cast)
			NodeColors::set_connection_color(connection, COLOR_TYPE::RUNTIME);
	}
}
}
