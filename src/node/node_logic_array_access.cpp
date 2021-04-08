#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_array_access(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeArrayAccess* node_arr_access = dynamic_cast<NodeArrayAccess*>(*it);

		if (!node_arr_access)
			continue;

		if (node_arr_access->m_current_val)
			delete node_arr_access->m_current_val;
		node_arr_access->m_current_val = nullptr;

		//get the "from" array
		NodeArray* from_node_array = nullptr;
		for (Connection& connection : node_arr_access->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			//check if two node_array_access are connected
			NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
			if (out_node_arr_access && out_node_arr_access != node_arr_access)
			{
				NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
				continue;
			}

			NodeArray* out_node_array = dynamic_cast<NodeArray*>(out_node);
			if (out_node_array)
			{
				//check if our index is within array bounds
				const int size_a = out_node_array->m_elements.size();
				const int size_b = out_node_array->m_other_elements.size();
				const unsigned int total_size = size_a + size_b;

				if (node_arr_access->m_index < total_size)
				{
					if (!from_node_array)
						from_node_array = out_node_array;
					else
						NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
				}
				else
				{
					NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
					continue;
				}
			}
		}

		if (!from_node_array)
		{
			//set the color for other connections (rhs)
			for (Connection& connection : node_arr_access->m_connections)
			{
				Node* out_node = static_cast<Node*>(connection.out_node);
				NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
				if (out_node_arr_access)
					NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
			}
			continue;
		}

		//check where we should get the element
		NodeValue* val_from_array = nullptr;
		if (node_arr_access->m_index < from_node_array->m_elements.size())
			val_from_array = from_node_array->m_elements[node_arr_access->m_index];
		else
		{
			int index = node_arr_access->m_index - from_node_array->m_elements.size();
			val_from_array = from_node_array->m_other_elements[index];
		}

		NodeValue* res_val = new NodeValue();
		res_val->copy(*val_from_array);
		node_arr_access->m_current_val = res_val;
	}
}
}
