#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"

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

		//store the result
		NodeVariable* res_node_var = nullptr;
		NodeVariable* from_node_var = nullptr;

		//make preliminary checks
		if (node_cast->m_connections.size() < 2)
			continue;

		//make sure node_cast has a connected node_var for the result
		for (const Connection& connection : node_cast->m_connections)
		{
			if (res_node_var && from_node_var)
				continue;

			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);

			NodeVariable* in_node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);

			NodeCast* in_node_cast = dynamic_cast<NodeCast*>(in_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

			if (in_node_var && out_node_cast)
			{
				res_node_var = in_node_var;
				continue;
			}

			if (in_node_cast && out_node_var)
			{
				from_node_var = out_node_var;
				continue;
			}
		}

		if (!res_node_var || !from_node_var)
			continue;

		NodeValue* from_val = &from_node_var->m_value;
		NodeValue* res_val = &res_node_var->m_value;
		res_val->cast_from(*from_val);
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
