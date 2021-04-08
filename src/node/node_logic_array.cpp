#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_math.hpp"
#include "node/node_op.hpp"
#include "node/node_array.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_array(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeArray* node_array = dynamic_cast<NodeArray*>(*it);

		if (!node_array)
			continue;

		node_array->m_other_elements.clear();
		const int size_a = node_array->m_elements.size();
		const int size_b = node_array->m_other_elements.size();
		const bool can_add = size_a + size_b < node_array->m_fixed_size;

		for (Connection& connection : node_array->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			if (out_node_var)
			{
				NodeValue* val = &out_node_var->m_value;
				if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
					continue;
				}
				node_array->m_other_elements.push_back(val);
			}

			NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);
			if (out_node_op)
			{
				NodeValue* val = out_node_op->m_current_val;
				if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
					continue;
				}
				node_array->m_other_elements.push_back(val);
			}

			NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
			if (out_node_math)
			{
				NodeValue* val = out_node_math->m_current_val;
				if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
					continue;
				}
				node_array->m_other_elements.push_back(val);
			}

			//this is the "from" array
			NodeArray* out_node_array = dynamic_cast<NodeArray*>(out_node);
			if (out_node_array && out_node_array != node_array)
			{
				if (node_array->m_array == +NODE_ARRAY::FIXED)
				{
					const int req_size = out_node_array->m_elements.size() +
						out_node_array->m_other_elements.size();
					const int cur_size = size_a + size_b;
					bool exceeds = req_size + cur_size > node_array->m_fixed_size;

					if (exceeds)
					{
						NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
						continue;
					}
				}

				for (int i = 0; i < out_node_array->m_elements.size(); i++)
				{
					NodeValue* val = out_node_array->m_elements[i];
					node_array->m_other_elements.push_back(val);
				}

				for (int i = 0; i < out_node_array->m_other_elements.size(); i++)
				{
					NodeValue* val = out_node_array->m_other_elements[i];
					node_array->m_other_elements.push_back(val);
				}
			}
		}
	}
}
}
