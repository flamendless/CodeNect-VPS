#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"
#include "node/node_array_access.hpp"

namespace CodeNect::NodeLogic
{
void process_cast(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeCast* node_cast = dynamic_cast<NodeCast*>(*it);

		if (!node_cast)
			continue;

		if (node_cast->m_current_val)
			delete node_cast->m_current_val;
		node_cast->m_current_val = nullptr;

		//store
		NodeValue* from_val = nullptr;

		//get the lhs
		for (const Connection& connection : node_cast->m_connections)
		{
			if (from_val)
				break;

			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);
			NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
			NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);

			if (out_node_var)
				from_val = &out_node_var->m_value;
			else if (out_node_op)
				from_val = out_node_op->m_current_val;
			else if (out_node_math)
				from_val = out_node_math->m_current_val;
			else if (out_node_arr_access)
				from_val = out_node_arr_access->m_current_val;
		}

		if (!from_val)
			continue;

		NODE_SLOT out_slot = NODE_SLOT::_from_string(node_cast->m_out_slots[0].title);

		NodeValue* res = new NodeValue();
		res->copy(out_slot);
		res->cast_from(*from_val);
		node_cast->m_current_val = res;
	}
}

bool validate_node_cast(Node* in_node, Node* out_node)
{
	//check if new connection from node_var to node_cast
	NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
	NodeCast* node_cast = dynamic_cast<NodeCast*>(in_node);

	if (node_var && node_cast)
	{
		//check if there is already a connection
		for (const Connection& connection : node_cast->m_connections)
		{
			NodeVariable* node_var = dynamic_cast<NodeVariable*>((Node*)connection.out_node);
			NodeCast* node_cast = dynamic_cast<NodeCast*>((Node*)connection.in_node);

			if (node_var && node_cast)
			{
				node_var->delete_connection(connection);
				node_cast->delete_connection(connection);
				return false;
			}
		}
	}

	return true;
}
}
