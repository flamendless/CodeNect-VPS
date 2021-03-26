#include "node/node_logic.hpp"

#include "node/nodes.hpp"

namespace CodeNect::NodeLogic
{
//this only processes the variable-variable connection such as assignment
//of value
void process_var(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(*it);

		if (!node_var)
			continue;

		//set to original value
		node_var->m_value = node_var->m_value_orig;

		//go through each connection
		for (const Connection& connection : node_var->m_connections)
		{
			//check for node_var -> node_var
			//in node is the "to"/"target" node (rhs)
			//out node is the "from" (lhs)
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* in_node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

			if (in_node_var && out_node_var)
			{
				if (node_var != out_node)
					node_var->m_value = out_node_var->m_value;
			}
		}
	}
}
}
