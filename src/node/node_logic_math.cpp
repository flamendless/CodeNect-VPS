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

		switch (node_math->m_math)
		{
			case NODE_MATH::EMPTY: break;
			case NODE_MATH::ROOT: NodeLogic::process_math_root(node_math); break;
			case NODE_MATH::POW: NodeLogic::process_math_power(node_math); break;
		}
	}
}

void process_math_lhs(NodeMath* node_math)
{
	if (node_math->m_connections.size() < 3)
		return;

	bool is_index_found = false;
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

			if (!is_index_found)
			{
				switch (val->m_slot)
				{
					case NODE_SLOT::EMPTY: break;
					case NODE_SLOT::BOOL: break;
					case NODE_SLOT::INTEGER: node_math->m_first = std::get<int>(val->data); break;
					case NODE_SLOT::FLOAT: node_math->m_first = std::get<float>(val->data); break;
					case NODE_SLOT::DOUBLE: node_math->m_first = std::get<double>(val->data); break;
					case NODE_SLOT::STRING: break;
				}
				is_index_found = true;
			}
			else
			{
				switch (val->m_slot)
				{
					case NODE_SLOT::EMPTY: break;
					case NODE_SLOT::BOOL: break;
					case NODE_SLOT::INTEGER: node_math->m_second = std::get<int>(val->data); break;
					case NODE_SLOT::FLOAT: node_math->m_second = std::get<float>(val->data); break;
					case NODE_SLOT::DOUBLE: node_math->m_second = std::get<double>(val->data); break;
					case NODE_SLOT::STRING: break;
				}
				is_index_found = false;
			}
		}
		else if (node_math_in && node_op)
		{
			//TODO
		}
	}
}

NodeVariable* get_res_var(NodeMath* node_math)
{
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
			return node_var;
		else if (node_math_out && node_op)
		{
			//TODO
		}
	}

	return nullptr;
}

void set_result(NodeValue* res, double result)
{
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

void process_math_root(NodeMath* node_math)
{
	process_math_lhs(node_math);
	NodeVariable* res_var = get_res_var(node_math);

	if (!res_var)
		return;

	//evaluate
	node_math->m_has_connections = true;
	NodeValue* res = &res_var->m_value;
	int index = 0;
	double result = 0;

	switch (node_math->m_in_slots[0].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: index = std::get<int>(node_math->m_first); break;
		case NODE_SLOT::FLOAT: index = (int)std::get<float>(node_math->m_first); break;
		case NODE_SLOT::DOUBLE: index = (int)std::get<double>(node_math->m_first); break;
		case NODE_SLOT::STRING: break;
	}

	switch (node_math->m_in_slots[node_math->m_in_slots.size() - 1].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			int radicand = std::get<int>(node_math->m_second);
			result = std::pow(radicand, 1.0/index);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			float radicand = std::get<float>(node_math->m_second);
			result = std::pow(radicand, 1.0/index);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			double radicand = std::get<double>(node_math->m_second);
			result = std::pow(radicand, 1.0/index);
			break;
		}
		case NODE_SLOT::STRING: break;
	}

	set_result(res, result);
}

void process_math_power(NodeMath* node_math)
{
	process_math_lhs(node_math);
	NodeVariable* res_var = get_res_var(node_math);

	if (!res_var)
		return;

	//evaluate
	node_math->m_has_connections = true;
	NodeValue* res = &res_var->m_value;
	double base = 0;
	double result = 0;

	switch (node_math->m_in_slots[0].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: base = (double)std::get<int>(node_math->m_first); break;
		case NODE_SLOT::FLOAT: base = (double)std::get<float>(node_math->m_first); break;
		case NODE_SLOT::DOUBLE: base = std::get<double>(node_math->m_first); break;
		case NODE_SLOT::STRING: break;
	}

	switch (node_math->m_in_slots[node_math->m_in_slots.size() - 1].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			int exponent = std::get<int>(node_math->m_second);
			result = std::pow(base, exponent);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			float exponent = std::get<float>(node_math->m_second);
			result = std::pow(base, exponent);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			double exponent = std::get<double>(node_math->m_second);
			result = std::pow(base, exponent);
			break;
		}
		case NODE_SLOT::STRING: break;
	}

	set_result(res, result);
}
}
