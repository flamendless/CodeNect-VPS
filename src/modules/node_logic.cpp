#include "modules/node_logic.hpp"
#include "modules/nodes.hpp"

namespace CodeNect
{
namespace NodeLogic
{
void process()
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin(); it != Nodes::v_nodes.end(); it++)
	{
		Node* node = *it;

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE: NodeLogic::process_var((NodeVariable*)node); break;
			case NODE_KIND::OPERATION: break;
			case NODE_KIND::IF: break;
		}
	}
}

void process_var(NodeVariable* node)
{
	node->m_value = &node->m_value_orig;

	//out node is "from"
	//in node is "to"
	for (const Connection& connection : node->m_connections)
	{
		Node* in_node = (Node*)connection.in_node;
		Node* out_node = (Node*)connection.out_node;
		NODE_KIND* in_kind = &in_node->m_kind;
		NODE_KIND* out_kind = &out_node->m_kind;

		if (*in_kind == +NODE_KIND::VARIABLE && *out_kind == +NODE_KIND::VARIABLE)
		{
			NodeVariable* out_node_var = (NodeVariable*)out_node;

			if (out_node_var != node)
				node->m_value = out_node_var->m_value;
		}
		else if (*in_kind == +NODE_KIND::VARIABLE && *out_kind == +NODE_KIND::OPERATION)
		{
			// NodeVariable* in_node_var = (NodeVariable*)in_node;
			NodeOp* out_node_op = (NodeOp*)out_node;

			perform_op(node, out_node_op);
		}
	}
}

//node here is the ouput "result" node
void op_add(NodeVariable* node, NodeOp* op)
{
	NODE_SLOT* out_slot = &node->m_value->m_slot;
	NodeValue res;
	res.copy(*out_slot);

	for (const Connection connection : op->m_connections)
	{
		Node* in_node = (Node*)connection.in_node;
		Node* out_node = (Node*)connection.out_node;
		NODE_KIND* in_kind = &in_node->m_kind;
		NODE_KIND* out_kind = &out_node->m_kind;

		if (*in_kind == +NODE_KIND::OPERATION && *out_kind == +NODE_KIND::VARIABLE)
		{
			NodeVariable* out_node_var = (NodeVariable*)out_node;

			res = res + *out_node_var->m_value;
		}
	}

	node->m_value = &res;
}

void perform_op(NodeVariable* node, NodeOp* op)
{
	switch (op->m_op)
	{
		case NODE_OP::EMPTY: break;
		case NODE_OP::ADD: op_add(node, op); break;
		case NODE_OP::SUB: break;
		case NODE_OP::MUL: break;
		case NODE_OP::DIV: break;
	}
}
}
}
