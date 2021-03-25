#include "node/node_logic.hpp"
#include "node/nodes.hpp"

namespace CodeNect::NodeLogic
{
struct Result
{
	std::vector<NodeVariable*> v_vars;
	NodeOperation* node_op;
	NodeVariable* node_var_res;
	NODE_SLOT slot_res = NODE_SLOT::EMPTY;
};

void process(void)
{
	NodeLogic::process_var();
	NodeLogic::process_op();
	NodeLogic::process_cast();
}

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

void process_op(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(*it);

		if (!node_op)
			continue;

		node_op->has_valid_connections = false;

		//store all the node_vars connected to this node_op
		Result res;
		res.node_op = node_op;

		//get the resulting node_var
		for (const Connection& connection : node_op->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* in_node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);

			if (in_node_var && out_node_op)
			{
				res.node_var_res = in_node_var;
				res.slot_res = in_node_var->m_value_orig.m_slot;
				break;
			}
		}

		//make sure that there is a "resulting" var connected
		if (res.slot_res == +NODE_SLOT::EMPTY)
			continue;

		//get all the lhs of the node_op
		for (const Connection& connection : node_op->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeOperation* in_node_op = dynamic_cast<NodeOperation*>(in_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

			if (in_node_op && out_node_var)
			{
				if (out_node_var->m_value_orig.m_slot == res.slot_res)
					res.v_vars.push_back(out_node_var);
			}
		}

		if (res.v_vars.size() < 2)
			continue;

		//here we are sure that they are of the same slot
		// process the v_vars and perform the operation
		node_op->has_valid_connections = true;
		NODE_OP* op = &node_op->m_op;
		NodeVariable* res_var = res.node_var_res;
		NodeValue result;
		bool is_first = true;

		for (NodeVariable* node_var : res.v_vars)
		{
			NodeValue* current_value = &node_var->m_value;

			if (is_first)
			{
				result.copy(*current_value);
				is_first = false;
				continue;
			}

			switch (*op)
			{
				case NODE_OP::EMPTY: break;
				case NODE_OP::ADD: result.add(*current_value); break;
				case NODE_OP::SUB: result.sub(*current_value); break;
				case NODE_OP::MUL: result.mul(*current_value); break;
				case NODE_OP::DIV: result.div(*current_value); break;
			}
		}

		res_var->m_value.copy(result);
	}
}

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
		NodeVariable* res_node_var;
		NodeVariable* from_node_var;

		//make preliminary checks
		if (node_cast->m_connections.size() < 2)
			continue;

		//make sure node_cast has a connected node_var for the result
		for (const Connection& connection : node_cast->m_connections)
		{
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
