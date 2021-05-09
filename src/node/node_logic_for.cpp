#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_for.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_size.hpp"
#include "node/node_array_access.hpp"

namespace CodeNect::NodeLogic
{
void process_for(NodeFor* node_for)
{
	//reset values
	node_for->m_cur_start_index = node_for->m_start_index;
	node_for->m_cur_end_index = node_for->m_end_index;
	node_for->m_cur_increment = node_for->m_increment;
	node_for->m_cur_it = node_for->m_override_it;

	NodeValue* value = nullptr;

	//get LHS for values
	for (const Connection& connection : node_for->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_for)
			continue;

		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);
		NodeCast* node_cast = dynamic_cast<NodeCast*>(out_node);
		NodeSize* node_size = dynamic_cast<NodeSize*>(out_node);
		NodeArrayAccess* node_array_access = dynamic_cast<NodeArrayAccess*>(out_node);

		if (node_var)
			value = &node_var->m_value;
		else if (node_math)
			value = node_math->m_current_val;
		else if (node_op)
			value = node_op->m_current_val;
		else if (node_cast)
			value = node_cast->m_current_val;
		else if (node_size)
			value = &node_size->m_val_size;
		else if (node_array_access)
			value = node_array_access->m_current_val;

		const char* slot = connection.in_slot;
		if (std::strcmp(slot, "INTEGER - start index") == 0)
			node_for->m_cur_start_index = std::get<int>(value->data);
		else if (std::strcmp(slot, "INTEGER - end index") == 0)
			node_for->m_cur_end_index = std::get<int>(value->data);
		else if (std::strcmp(slot, "INTEGER - increment") == 0)
			node_for->m_cur_increment = std::get<int>(value->data);
	}

	//manage iterator output
	for (const Connection& connection : node_for->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		Node* in_node = static_cast<Node*>(connection.in_node);
		if (out_node != node_for)
			continue;
		const char* slot = connection.out_slot;
		if (std::strcmp(slot, "INTEGER - iterator") == 0)
		{
			NodeVariable* node_var = static_cast<NodeVariable*>(in_node);
			//TODO FIX, add override to NodeVariable?
			node_var->m_value.set((int)node_for->m_cur_it);
		}
	}
}

bool validate_node_for(Node* in_node, Node* out_node)
{
	NodeFor* node_for = dynamic_cast<NodeFor*>(out_node);
	NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);

	if (node_for && node_var)
	{
		const char* out_it = node_for->m_out_slots[0].title;
		bool same = std::strcmp(out_it, "INTEGER - iterator") == 0;
		if (!same)
		{
			Alert::open_for_docs("Iterator can only be connected to Variable Node", DOC_ID::IT_MUST_BE_VAR);
			return false;
		}
	}

	return true;
}
}
