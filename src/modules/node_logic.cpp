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
		NODE_OP* op = &node_op->m_op;
		NodeVariable* res_var = res.node_var_res;
		NodeValue* res_val = &res_var->m_value;

		for (NodeVariable* node_var : res.v_vars)
		{
			NodeValue* current_value = &node_var->m_value;

			switch (*op)
			{
				case NODE_OP::EMPTY: break;
				case NODE_OP::ADD:
				{
					res_val->add(*current_value);
					break;
				}
				case NODE_OP::SUB:
				{
					res_val->sub(*current_value);
					break;
				}
				case NODE_OP::MUL:
				{
					res_val->mul(*current_value);
					break;
				}
				case NODE_OP::DIV:
				{
					res_val->div(*current_value);
					break;
				}
			}
		}
	}
}
}
