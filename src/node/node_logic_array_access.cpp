#include "node/node_logic.hpp"

#include "node/node_var.hpp"
#include "node/nodes.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
bool index_within_array_bounds(NodeArray* node_array, const int index)
{
	const int size_a = node_array->m_elements.size();
	const int size_b = node_array->m_other_elements.size();
	const unsigned int total_size = size_a + size_b;

	return index < total_size;
}

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
		node_arr_access->m_index = node_arr_access->m_index_orig;
		node_arr_access->m_has_array = false;

		//first check if there's a node_var connected
		for (Connection& connection : node_arr_access->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			if (out_node_var)
			{
				const int index = std::get<int>(out_node_var->m_value.data);
				node_arr_access->m_index = index;
			}
		}

		//get the "from" array
		NodeArray* from_node_array = nullptr;
		for (Connection& connection : node_arr_access->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeArray* out_node_array = dynamic_cast<NodeArray*>(out_node);
			if (out_node_array)
			{
				bool res = NodeLogic::index_within_array_bounds(out_node_array, node_arr_access->m_index);
				if (res)
				{
					node_arr_access->m_has_array = true;
					from_node_array = out_node_array;
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

bool validate_node_array_access(Node* in_node, Node* out_node)
{
	//connection must be from node_array or node_var to node_array_access only
	NodeArray* node_array = dynamic_cast<NodeArray*>(out_node);
	NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
	NodeArrayAccess* node_arr_access = dynamic_cast<NodeArrayAccess*>(in_node);

	if (node_arr_access)
	{
		if (node_array || node_var)
			return true;
		else
			return false;
	}

	return true;
}
}
