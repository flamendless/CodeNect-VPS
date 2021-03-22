#include "modules/node_logic.hpp"
#include "modules/nodes.hpp"

namespace CodeNect::NodeLogic
{
void process(void)
{
	NodeLogic::process_var();
	NodeLogic::process_op();
}

//this only processes the variable-variable connection such as assignment
//of value
void process_var(void)
{
	for (std::vector<NodeVariable*>::iterator it = Nodes::v_nodes_var.begin();
		it != Nodes::v_nodes_var.end();
		it++)
	{
		NodeVariable* node_var = *it;

		//set to original value
		node_var->m_value = &node_var->m_value_orig;

		//go through each connection
		for (const Connection& connection : node_var->m_connections)
		{
			//in node is the "to"/"target" node (rhs)
			//out node is the "from" (lhs)
			Node* in_node = (Node*)connection.in_node;
			Node* out_node = (Node*)connection.out_node;
			NODE_KIND* in_kind = &in_node->m_kind;
			NODE_KIND* out_kind = &out_node->m_kind;

			bool is_in_var = *in_kind == +NODE_KIND::VARIABLE;
			bool is_out_var = *out_kind == +NODE_KIND::VARIABLE;

			if (is_in_var && is_out_var)
			{
				NodeVariable* out_node_var = (NodeVariable*)out_node;

				if (node_var != out_node)
					node_var->m_value = out_node_var->m_value;
			}
		}
	}
}

void process_op(void)
{
}
}
