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

		switch (node_math->m_math)
		{
			case NODE_MATH::EMPTY: break;
			case NODE_MATH::ROOT: NodeLogic::process_math_root(node_math); break;
		}
	}
}

void process_math_root(NodeMath* node_math)
{
	MathRoot* data = &std::get<MathRoot>(node_math->m_data);
	data->m_index = 0;
	data->m_radicand = 0;
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
			if (!is_index_found)
			{
				int index = std::get<int>(node_var->m_value.data);
				data->m_index = index;
				is_index_found = true;
			}
			else
			{
				NodeValue* val = &node_var->m_value;
				switch (val->m_slot)
				{
					case NODE_SLOT::EMPTY: break;
					case NODE_SLOT::BOOL: break;
					case NODE_SLOT::INTEGER: data->m_radicand = std::get<int>(val->data); break;
					case NODE_SLOT::FLOAT: data->m_radicand = std::get<float>(val->data); break;
					case NODE_SLOT::DOUBLE: data->m_radicand = std::get<double>(val->data); break;
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
	data->m_has_connections = true;
	NodeValue* res = &res_var->m_value;

	switch (res->m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER:
		{
			int radicand = std::get<int>(data->m_radicand);
			int result = std::pow(radicand, 1.0/data->m_index);
			res->set((int)result);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			float radicand = std::get<float>(data->m_radicand);
			float result = (float)std::pow(radicand, 1.0/data->m_index);
			res->set(result);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			double radicand = std::get<double>(data->m_radicand);
			double result = (double)std::pow(radicand, 1.0/data->m_index);
			res->set(result);
			break;
		}
		case NODE_SLOT::STRING: break;
	}
}
}
