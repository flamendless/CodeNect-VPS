#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_size.hpp"
#include "node/node_colors.hpp"
#include "node/node_string.hpp"
#include "modules/debugger.hpp"

namespace CodeNect::NodeLogic
{
void process_op(NodeOperation* node_op)
{
	node_op->m_has_valid_connections = false;

	if (node_op->m_current_val)
		delete node_op->m_current_val;
	node_op->m_current_val = nullptr;

	bool has_res = false;
	std::vector<NodeValue*> v_values;

	//get all the lhs of the node_op
	for (const Connection& connection : node_op->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_op)
		{
			has_res = true;
			continue;
		}

		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
		NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
		NodeSize* out_node_size = dynamic_cast<NodeSize*>(out_node);

		if (out_node_var)
			v_values.push_back(&out_node_var->m_value);
		else if (out_node_math)
		{
			if (out_node_math->m_current_val)
				v_values.push_back(out_node_math->m_current_val);
		}
		else if (out_node_arr_access)
		{
			if (out_node_arr_access->m_current_val)
				v_values.push_back(out_node_arr_access->m_current_val);
		}
		else if (out_node_size)
			v_values.push_back(&out_node_size->m_val_size);
	}

	if (v_values.size() < 2)
	{
		Debugger::add_message(std::move("NodeOperation needs at least 2 inputs"),
				OUTPUT_TYPE::WARNING, node_op, DOC_ID::OP_REQ);
		for (Connection& connection : node_op->m_connections)
			NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
		return;
	}

	//make sure that there is a "resulting" var connected
	if (!has_res)
	{
		Debugger::add_message(std::move("NodeOperation does not have any output connection"),
				OUTPUT_TYPE::WARNING, node_op, DOC_ID::OP_REQ);
		return;
	}

	//here we are sure that they are of the same slot
	// process the v_values and perform the operation
	node_op->m_has_valid_connections = true;
	NODE_OP* op = &node_op->m_op;
	NodeValue result;
	bool is_first = true;

	for (NodeValue* &value : v_values)
	{
		if (is_first)
		{
			result.copy(*value);
			is_first = false;
			continue;
		}

		switch (*op)
		{
			case NODE_OP::EMPTY: break;
			case NODE_OP::ADD: result.add(*value); break;
			case NODE_OP::SUB: result.sub(*value); break;
			case NODE_OP::MUL: result.mul(*value); break;
			case NODE_OP::DIV: result.div(*value); break;
			case NODE_OP::MOD: result.mod(*value); break;
		}
	}

	node_op->m_current_val = new NodeValue();
	node_op->m_current_val->copy(result);
}
}
