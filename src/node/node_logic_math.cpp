#include "node/node_logic.hpp"

#include <cmath>
#include "ppk_assert.h"
#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"
#include "node/node_array_access.hpp"
#include "node/node_size.hpp"

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

		if (node_math->m_current_val)
			delete node_math->m_current_val;
		node_math->m_current_val = nullptr;

		if (node_math->m_in_slots.size() == 0)
			return;

		bool is_first_found = false;

		//get lhs of this node_math
		for (const Connection& connection : node_math->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);
			NodeArrayAccess* node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
			NodeSize* node_size = dynamic_cast<NodeSize*>(out_node);
			NodeValue* val = nullptr;

			if (node_var)
				val = &node_var->m_value;
			else if (node_op && node_op->m_current_val)
				val = node_op->m_current_val;
			else if (node_arr_access && node_arr_access->m_current_val)
				val = node_arr_access->m_current_val;
			else if (node_size)
				val = &node_size->m_val_size;

			if (val)
			{
				NODE_SLOT* slot = &val->m_slot;
				std::variant<int, float, double>* var = nullptr;

				if (!is_first_found && *slot == node_math->m_in_slots[0].kind)
				{
					var = &node_math->m_first;
					is_first_found = node_math->m_needs_two_val && true;
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
		}

		//get node_var rhs
		NodeVariable* res_var = nullptr;
		NodeOperation* res_op = nullptr;

		for (const Connection& connection : node_math->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeOperation* node_op = dynamic_cast<NodeOperation*>(in_node);

			if (node_var)
				res_var = node_var;
			else if (node_op)
				res_op = node_op;
		}

		//evaluate
		node_math->m_has_connections = true;
		NodeValue* res = new NodeValue();

		if (res_var)
			res->copy(res_var->m_value);
		else
		{
			NODE_SLOT slot = NODE_SLOT::_from_string(node_math->m_out_slots[0].title);
			res->copy(slot);
		}

		double result = NodeLogic::calculate_math(node_math, NodeMath::m_functions[node_math->m_math._to_string()]);

		switch (res->m_slot)
		{
			case NODE_SLOT::EMPTY: PPK_ASSERT(false, "this should not be reached"); break;
			case NODE_SLOT::BOOL: PPK_ASSERT(false, "this should not be reached"); break;
			case NODE_SLOT::INTEGER: res->set((int)result); break;
			case NODE_SLOT::FLOAT: res->set((float)result); break;
			case NODE_SLOT::DOUBLE: res->set((double)result); break;
			case NODE_SLOT::STRING: PPK_ASSERT(false, "this should not be reached"); break;
		}

		node_math->m_current_val = res;
		if (res_var)
			res_var->m_value.copy(*res);
	}
}

double calculate_math(NodeMath* node_math, std::function<double(double a, double b)> fn)
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
}
