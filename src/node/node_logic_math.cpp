#include "node/node_logic.hpp"

#include <cmath>
#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"

namespace CodeNect::NodeLogic
{
void process_math(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeMath* node_math = dynamic_cast<NodeMath*>(*it);

		if (!node_math)
			continue;

		node_math->reset();
		NodeLogic::calculate_math(node_math);
	}
}

double calculate(NodeMath* node_math, std::function<double(double a, double b)> fn)
{
	double a = 0;
	double result = 0;

	switch (node_math->m_in_slots[0].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: a = std::get<int>(node_math->m_first); break;
		case NODE_SLOT::FLOAT: a = std::get<float>(node_math->m_first); break;
		case NODE_SLOT::DOUBLE: a = std::get<double>(node_math->m_first); break;
		case NODE_SLOT::STRING: break;
	}

	switch (node_math->m_in_slots[node_math->m_in_slots.size() - 1].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			int b = std::get<int>(node_math->m_second);
			result = fn(a, b);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			float b = std::get<float>(node_math->m_second);
			result = fn(a, b);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			double b = std::get<double>(node_math->m_second);
			result = fn(a, b);
			break;
		}
		case NODE_SLOT::STRING: break;
	}

	return result;
}

void calculate_math(NodeMath* node_math)
{
	if (node_math->m_in_slots.size() == 0)
		return;

	bool is_first_found = false;

	//get lhs of this node_math
	for (const Connection& connection : node_math->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		Node* out_node = static_cast<Node*>(connection.out_node);
		//can be node_var or node_op
		NodeMath* node_math_in = dynamic_cast<NodeMath*>(in_node);
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);

		if (node_math_in && node_var)
		{
			NodeValue* val = &node_var->m_value;
			NODE_SLOT* slot = &val->m_slot;
			std::variant<int, float, double>* var = nullptr;

			if (!is_first_found && *slot == node_math->m_in_slots[0].kind)
			{
				var = &node_math->m_first;
				is_first_found = true;
			}
			else if (*slot == node_math->m_in_slots[node_math->m_in_slots.size() - 1].kind)
			{
				var = &node_math->m_second;
				is_first_found = false;
			}

			switch (val->m_slot)
			{
				case NODE_SLOT::EMPTY: break;
				case NODE_SLOT::BOOL: break;
				case NODE_SLOT::INTEGER: *var = std::get<int>(val->data); break;
				case NODE_SLOT::FLOAT: *var = std::get<float>(val->data); break;
				case NODE_SLOT::DOUBLE: *var = std::get<double>(val->data); break;
				case NODE_SLOT::STRING: break;
			}
		}
		else if (node_math_in && node_op)
		{
			//TODO
		}
	}

	NodeVariable* res_var = nullptr;
	//get rhs
	for (const Connection& connection : node_math->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		Node* out_node = static_cast<Node*>(connection.out_node);
		//lhs
		NodeMath* node_math_out = dynamic_cast<NodeMath*>(out_node);
		//rhs
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(in_node);

		if (node_math_out && node_var)
			res_var = node_var;
		else if (node_math_out && node_op)
		{
			//TODO
		}
	}

	if (!res_var)
		return;

	//evaluate
	node_math->m_has_connections = true;
	NodeValue* res = &res_var->m_value;
	double result = calculate(node_math, NodeMath::m_functions[node_math->m_math._to_string()]);

	switch (res->m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: res->set((int)result); break;
		case NODE_SLOT::FLOAT: res->set((float)result); break;
		case NODE_SLOT::DOUBLE: res->set((double)result); break;
		case NODE_SLOT::STRING: break;
	}
}
}
