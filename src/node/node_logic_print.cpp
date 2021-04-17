#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_print.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_size.hpp"

namespace CodeNect::NodeLogic
{
void process_print(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodePrint* node_print = dynamic_cast<NodePrint*>(*it);

		if (!node_print)
			continue;

		node_print->m_str = node_print->m_orig_str;

		NodeValue* from_val = nullptr;
		//get the value of connected node_var and set to ours (lhs)
		if (node_print->m_override)
		{
			for (const Connection& connection : node_print->m_connections)
			{
				Node* out_node = static_cast<Node*>(connection.out_node);
				NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
				NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
				NodeSize* out_node_size = dynamic_cast<NodeSize*>(out_node);

				if (out_node_var)
					from_val = &out_node_var->m_value;
				else if (out_node_arr_access)
					from_val = out_node_arr_access->m_current_val;
				else if (out_node_size)
					from_val = &out_node_size->m_val_size;

				if (from_val)
				{
					std::string str;
					switch (from_val->m_slot)
					{
						case NODE_SLOT::EMPTY: break;
						case NODE_SLOT::BOOL: str = std::get<bool>(from_val->data) ? "true" : "false"; break;
						case NODE_SLOT::INTEGER: str = std::to_string(std::get<int>(from_val->data)); break;
						case NODE_SLOT::FLOAT: str = std::to_string(std::get<float>(from_val->data)); break;
						case NODE_SLOT::DOUBLE: str = std::to_string(std::get<double>(from_val->data)); break;
						case NODE_SLOT::STRING: str = std::get<std::string>(from_val->data); break;
					}
					node_print->m_str = str;
				}
			}
		}
	}
}

//node_print can not be connected node_array
bool validate_node_print(Node* in_node, Node* out_node)
{
	NodeArray* node_array = dynamic_cast<NodeArray*>(out_node);

	if (node_array)
		return false;

	return true;
}
}
