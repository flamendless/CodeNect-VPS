#include "node/node_logic.hpp"

#include "node/node_var.hpp"
#include "node/nodes.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"
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

void process_array_access(NodeArrayAccess* node_arr_access)
{
	if (node_arr_access->m_current_val)
		delete node_arr_access->m_current_val;
	node_arr_access->m_current_val = nullptr;
	node_arr_access->m_index = node_arr_access->m_index_orig;
	node_arr_access->m_has_array = false;

	//first check if there's a node connected that will set the index
	for (Connection& connection : node_arr_access->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);
		NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
		NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);
		int index = node_arr_access->m_index_orig;

		if (out_node_var)
			index = std::get<int>(out_node_var->m_value.data);
		else if (out_node_cast)
		{
			if (!out_node_cast->m_current_val)
				continue;
			index = std::get<int>(out_node_cast->m_current_val->data);
		}
		else if (out_node_math)
		{
			if (!out_node_math->m_current_val)
				continue;
			index = std::get<int>(out_node_math->m_current_val->data);
		}
		else if (out_node_op)
		{
			if (!out_node_op->m_current_val)
				continue;
			index = std::get<int>(out_node_op->m_current_val->data);
		}

		node_arr_access->m_index = index;
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
				NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
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
				NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
		}
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
