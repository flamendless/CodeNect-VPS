#include "modules/node_logic.hpp"
#include "modules/nodes.hpp"

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
		node_var->m_value = node_var->m_value_orig;

		//go through each connection
		for (const Connection& connection : node_var->m_connections)
		{
			//check for node_var -> node_var
			//in node is the "to"/"target" node (rhs)
			//out node is the "from" (lhs)
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NODE_KIND* in_kind = &in_node->m_kind;
			NODE_KIND* out_kind = &out_node->m_kind;

			bool is_in_var = *in_kind == +NODE_KIND::VARIABLE;
			bool is_out_var = *out_kind == +NODE_KIND::VARIABLE;

			if (is_in_var && is_out_var)
			{
				NodeVariable* out_node_var = static_cast<NodeVariable*>(out_node);

				if (node_var != out_node)
					node_var->m_value = out_node_var->m_value;
			}
		}
	}
}

void process_op(void)
{
	for (std::vector<NodeOperation*>::iterator it = Nodes::v_nodes_op.begin();
		it != Nodes::v_nodes_op.end();
		it++)
	{
		NodeOperation* node_op = *it;
		node_op->has_valid_connections = false;

		//store all the node_vars connected to this node_op
		Result res;
		res.node_op = node_op;

		//get the resulting node_var
		for (const Connection& connection : node_op->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NODE_KIND* in_kind = &in_node->m_kind;
			NODE_KIND* out_kind = &out_node->m_kind;
			bool is_out_op = *out_kind == +NODE_KIND::OPERATION;
			bool is_in_var = *in_kind == +NODE_KIND::VARIABLE;

			if (is_out_op && is_in_var)
			{
				NodeVariable* node_var_res = static_cast<NodeVariable*>(in_node);
				res.node_var_res = node_var_res;
				res.slot_res = node_var_res->m_value_orig.m_slot;
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
			NODE_KIND* in_kind = &in_node->m_kind;
			NODE_KIND* out_kind = &out_node->m_kind;
			bool is_in_op = *in_kind == +NODE_KIND::OPERATION;
			bool is_out_var = *out_kind == +NODE_KIND::VARIABLE;

			if (is_in_op && is_out_var)
			{
				NodeVariable* node_var = static_cast<NodeVariable*>(out_node);

				if (node_var->m_value_orig.m_slot == res.slot_res)
					res.v_vars.push_back(node_var);
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
				case NODE_OP::ADD:
				{
					result.add(*current_value);
					break;
				}
				case NODE_OP::SUB:
				{
					result.sub(*current_value);
					break;
				}
				case NODE_OP::MUL:
				{
					result.mul(*current_value);
					break;
				}
				case NODE_OP::DIV:
				{
					result.div(*current_value);
					break;
				}
			}
		}

		res_var->m_value.copy(result);
	}
}
}
