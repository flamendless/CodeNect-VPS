#include "node/node_logic.hpp"

#include "node/node_set.hpp"
#include "node/node_var.hpp"

namespace CodeNect::NodeLogic
{
void process_set(NodeSet* node_set)
{
	//reset
	node_set->m_node_val = node_set->m_node_val_orig;

	//get LHS
	for (const Connection& connection : node_set->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_set)
			continue;

		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		if (node_var)
			node_set->m_node_val.copy(node_var->m_value);
	}

	//set the rhs
	node_set->m_node_var->m_value.copy(node_set->m_node_val);
}
}
