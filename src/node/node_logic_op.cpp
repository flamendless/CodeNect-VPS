#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"
#include "node/node_array.hpp"

namespace CodeNect::NodeLogic
{
struct Result
{
	std::vector<NodeValue*> v_values;
	NodeOperation* node_op;
	std::variant<NodeVariable*, NodeMath*, NodeArray*> node_res;
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

		if (node_op->m_current_val)
			delete node_op->m_current_val;
		node_op->m_current_val = nullptr;

		//store all the node_vars connected to this node_op
		Result res;
		res.node_op = node_op;

		//get the resulting node_var
		for (const Connection& connection : node_op->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* in_node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeMath* in_node_math = dynamic_cast<NodeMath*>(in_node);
			NodeArray* in_node_array = dynamic_cast<NodeArray*>(in_node);
			NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);

			if (in_node_var && out_node_op)
			{
				res.node_res = in_node_var;
				res.slot_res = in_node_var->m_value_orig.m_slot;
				break;
			}
			else if (in_node_math && out_node_op)
			{
				res.node_res = in_node_math;
				res.slot_res = NODE_SLOT::_from_string(out_node_op->m_in_slots[0].title);
				break;
			}
			else if (in_node_array && out_node_op)
			{
				res.node_res = in_node_array;
				res.slot_res = NODE_SLOT::_from_string(out_node_op->m_in_slots[0].title);
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
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);

			if (out_node_var)
			{
				if (out_node_var->m_value_orig.m_slot == res.slot_res)
					res.v_values.push_back(&out_node_var->m_value);
			}
			else if (out_node_math)
			{
				if (out_node_math->m_out_slots[0].kind == res.slot_res)
				{
					if (out_node_math->m_current_val)
						res.v_values.push_back(out_node_math->m_current_val);
				}
			}
		}

		if (res.v_values.size() < 2)
			continue;

		//here we are sure that they are of the same slot
		// process the v_values and perform the operation
		node_op->m_has_valid_connections = true;
		NODE_OP* op = &node_op->m_op;
		NodeValue result;
		bool is_first = true;

		for (NodeValue* value : res.v_values)
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

		switch (res.node_res.index())
		{
			case 0:
			{
				NodeVariable* res_var = std::get<NodeVariable*>(res.node_res);
				res_var->m_value.copy(result);
				break;
			}
			case 1: break;
			case 2: break;
		}

		node_op->m_current_val = new NodeValue();
		node_op->m_current_val->copy(result);
	}
}
}
