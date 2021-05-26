#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"
#include "node/node_array_access.hpp"
#include "node/node_size.hpp"
#include "node/node_string.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_op.hpp"
#include "node/node_cmp.hpp"

namespace CodeNect::NodeLogic
{
//this only processes the variable-variable connection such as assignment
//of value
void process_var(NodeVariable* node_var)
{
	//set to original value
	int temp;
	if (node_var->m_value.m_slot == +NODE_SLOT::INTEGER)
		temp = std::get<int>(node_var->m_value.data);

	node_var->m_value = node_var->m_value_orig;

	//go through each connection
	for (const Connection& connection : node_var->m_connections)
	{
		if (!connection.is_valid)
			continue;

		//check for node_var -> node_var
		//in node is the "to"/"target" node (rhs)
		//out node is the "from" (lhs)
		Node* out_node = static_cast<Node*>(connection.out_node);
		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);
		NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
		NodeSize* out_node_size = dynamic_cast<NodeSize*>(out_node);
		NodeString* out_node_str = dynamic_cast<NodeString*>(out_node);
		NodePrint* out_node_print = dynamic_cast<NodePrint*>(out_node);
		NodePrompt* out_node_prompt = dynamic_cast<NodePrompt*>(out_node);
		NodeFor* out_node_for = dynamic_cast<NodeFor*>(out_node);
		NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);
		NodeComparison* out_node_cmp = dynamic_cast<NodeComparison*>(out_node);

		if (out_node_var)
		{
			if (node_var != out_node)
				node_var->m_value = out_node_var->m_value;
		}
		else if (out_node_cast)
		{
			if (out_node_cast->m_current_val)
				node_var->m_value = *out_node_cast->m_current_val;
		}
		else if (out_node_arr_access)
		{
			if (out_node_arr_access->m_current_val)
				node_var->m_value = *out_node_arr_access->m_current_val;
		}
		else if (out_node_size)
			node_var->m_value.set((int)out_node_size->m_size);
		else if (out_node_str)
			node_var->m_value.set((std::string)out_node_str->m_current_str);
		else if (out_node_print)
			node_var->m_value.set((std::string)out_node_print->m_str);
		else if (out_node_prompt)
			node_var->m_value.set((std::string)out_node_prompt->m_fake_input.get_value_str());
		else if (out_node_for)
		{
			const char* slot_in = connection.in_slot;
			if (std::strcmp(slot_in, "INTEGER - iterator"))
				node_var->m_value.set((int)temp);
		}
		else if (out_node_op)
		{
			if (out_node_op->m_current_val)
				node_var->m_value.copy(*out_node_op->m_current_val);
		}
		else if (out_node_cmp)
			node_var->m_value.set(out_node_cmp->current_res);
	}
}
}
