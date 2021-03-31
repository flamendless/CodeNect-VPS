#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_print.hpp"

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

		//get the value of connected node_var and set to ours
		if (node_print->m_override)
		{
			for (const Connection& connection : node_print->m_connections)
			{
				Node* in_node = static_cast<Node*>(connection.in_node);
				Node* out_node = static_cast<Node*>(connection.out_node);
				NodePrint* in_node_print = dynamic_cast<NodePrint*>(in_node);
				NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

				//check if connected is node_var
				if (in_node_print && out_node_var)
				{
					NodeValue* val = &out_node_var->m_value;
					std::string str;

					switch (val->m_slot)
					{
						case NODE_SLOT::EMPTY: break;
						case NODE_SLOT::BOOL: str = std::get<bool>(val->data) ? "true" : "false"; break;
						case NODE_SLOT::INTEGER: str = std::to_string(std::get<int>(val->data)); break;
						case NODE_SLOT::FLOAT: str = std::to_string(std::get<float>(val->data)); break;
						case NODE_SLOT::DOUBLE: str = std::to_string(std::get<double>(val->data)); break;
						case NODE_SLOT::STRING: str = std::get<std::string>(val->data); break;
					}

					node_print->m_str = str;
				}
			}
		}
	}
}
}
