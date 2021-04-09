#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"
#include "node/node_array_access.hpp"

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
			if (!connection.is_valid)
				continue;

			//check for node_var -> node_var
			//in node is the "to"/"target" node (rhs)
			//out node is the "from" (lhs)
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);
			NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);

			if (out_node_var)
			{
				if (node_var != out_node)
					node_var->m_value = out_node_var->m_value;
			}
			else if (out_node_cast)
			{
				if (out_node_cast->m_current_val)
					node_var->m_value = *out_node_cast->m_current_val;
			}
			else if (out_node_arr_access)
			{
				if (out_node_arr_access->m_current_val)
					node_var->m_value = *out_node_arr_access->m_current_val;
			}
		}
	}
}
}
