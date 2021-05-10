#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"
#include "node/node_array_access.hpp"
#include "node/node_size.hpp"
#include "node/node_prompt.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_cast(NodeCast* node_cast)
{
	if (node_cast->m_current_val)
		delete node_cast->m_current_val;
	node_cast->m_current_val = nullptr;

	//store
	NodeValue* from_val = nullptr;
	Connection* conn = nullptr;

	//get the lhs
	for (Connection& connection : node_cast->m_connections)
	{
		if (conn)
			break;

		Node* out_node = static_cast<Node*>(connection.out_node);
		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);
		NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
		NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
		NodeSize* out_node_size = dynamic_cast<NodeSize*>(out_node);
		NodePrompt* out_node_prompt = dynamic_cast<NodePrompt*>(out_node);

		if (out_node_var)
			from_val = &out_node_var->m_value;
		else if (out_node_op)
			from_val = out_node_op->m_current_val;
		else if (out_node_math)
			from_val = out_node_math->m_current_val;
		else if (out_node_arr_access)
			from_val = out_node_arr_access->m_current_val;
		else if (out_node_size)
			from_val = &out_node_size->m_val_size;
		else if (out_node_prompt)
			from_val = &out_node_prompt->m_fake_input;

		conn = &connection;
	}

	if (!from_val)
		return;

	NODE_SLOT out_slot = NODE_SLOT::_from_string(node_cast->m_out_slots[0].title);

	//set rhs connection
	if (out_slot == +NODE_SLOT::STRING)
	{
		for (Connection& connection : node_cast->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);
			if (out_node_cast == node_cast)
				NodeColors::set_connection_color(connection, COLOR_TYPE::RUNTIME);
		}
	}

	NodeValue* res = new NodeValue();
	res->copy(out_slot);
	bool success = res->cast_from(*from_val);
	if (!success)
		NodeColors::set_connection_color(*conn, COLOR_TYPE::FAIL);
	node_cast->m_current_val = res;
}
}
