#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_math.hpp"
#include "node/node_op.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_string.hpp"
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

		if (node_array->m_from_string.size() != 0)
		{
			for (std::vector<NodeValue*>::iterator it = node_array->m_from_string.begin();
				it != node_array->m_from_string.end();
				it++)
			{
				delete *it;
			}
			node_array->m_from_string.clear();
		}
		node_array->m_other_elements.clear();
		const int size_a = node_array->m_elements.size();
		const int size_b = node_array->m_other_elements.size();
		const bool can_add = size_a + size_b < node_array->m_fixed_size;

		for (Connection& connection : node_array->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);
			NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
			NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
			NodeArray* out_node_array = dynamic_cast<NodeArray*>(out_node); //"from" array
			NodeString* out_node_str = dynamic_cast<NodeString*>(out_node);

			if (out_node_var)
			{
				NodeValue* val = &out_node_var->m_value;
				if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
					continue;
				}
				node_array->m_other_elements.push_back(val);
			}
			else if (out_node_op)
			{
				NodeValue* val = out_node_op->m_current_val;
				if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
					continue;
				}
				node_array->m_other_elements.push_back(val);
			}
			else if (out_node_math)
			{
				NodeValue* val = out_node_math->m_current_val;
				if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
					continue;
				}
				node_array->m_other_elements.push_back(val);
			}
			else if (out_node_arr_access)
			{
				if (out_node_arr_access->m_current_val)
				{
					if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
					{
						NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
						continue;
					}
					node_array->m_other_elements.push_back(out_node_arr_access->m_current_val);
				}
			}
			else if (out_node_str)
			{
				for (char& ch : out_node_str->m_from_str)
				{
					if (node_array->m_array == +NODE_ARRAY::FIXED && !can_add)
					{
						NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
						break;
					}
					std::string str_ch(1, ch);
					NodeValue* val = new NodeValue();
					val->set((std::string)str_ch);
					node_array->m_from_string.push_back(val);
				}
			}
			else if (out_node_array && out_node_array != node_array)
			{
				const int req_size = out_node_array->m_elements.size() +
					out_node_array->m_other_elements.size();
				const int cur_size = size_a + size_b;

				if (node_array->m_array == +NODE_ARRAY::FIXED)
				{
					bool exceeds = req_size + cur_size > node_array->m_fixed_size;

					if (exceeds)
					{
						NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
						continue;
					}
				}

				node_array->m_other_elements.reserve(req_size + cur_size);
				node_array->m_other_elements.insert(node_array->m_other_elements.end(),
					out_node_array->m_elements.begin(),
					out_node_array->m_elements.end());
				node_array->m_other_elements.insert(node_array->m_other_elements.end(),
					out_node_array->m_other_elements.begin(),
					out_node_array->m_other_elements.end());
			}
		}
	}
}

bool validate_node_array(Node* in_node, Node* out_node)
{
	NodeArray* node_array = dynamic_cast<NodeArray*>(out_node);
	NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);

	if (node_array && node_var)
	{
		Alert::open_for_docs("Variable Node can't be assigned an array", DOC_ID::CANT_REF_ARRAY);
		return false;
	}

	return true;
}
}
