#include "node/node_logic.hpp"

#include "node/node_set.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_math.hpp"
#include "modules/debugger.hpp"

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
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);
		NodeCast* node_cast = dynamic_cast<NodeCast*>(out_node);
		NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);

		if (node_var)
			node_set->m_node_val.copy(node_var->m_value);
		else if (node_op)
			node_set->m_node_val.copy(*node_op->m_current_val);
		else if (node_cast)
			node_set->m_node_val.copy(*node_cast->m_current_val);
		else if (node_math)
			node_set->m_node_val.copy(*node_math->m_current_val);
	}

	//set the rhs
	if (node_set->m_node_var)
		node_set->m_node_var->m_value.copy(node_set->m_node_val);
	else
		Debugger::add_message(std::move("Missing target variable. Perhaps was deleted?"), OUTPUT_TYPE::ERR);
}
}
