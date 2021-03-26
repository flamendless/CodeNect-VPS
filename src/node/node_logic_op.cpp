#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"

namespace CodeNect::NodeLogic
{
struct Result
{
	std::vector<NodeVariable*> v_vars;
	NodeOperation* node_op;
	NodeVariable* node_var_res;
	NODE_SLOT slot_res = NODE_SLOT::EMPTY;
};

void process_op(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(*it);

		if (!node_op)
			continue;

		node_op->m_has_valid_connections = false;

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
		node_op->m_has_valid_connections = true;
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
}
