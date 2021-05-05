#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_size.hpp"
#include "node/node_array.hpp"
#include "node/node_var.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_size(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeSize* node_size = dynamic_cast<NodeSize*>(*it);

		if (!node_size)
			continue;

		node_size->m_size = 0;
		node_size->m_val_size.set((int)0);

		//get the lhs
		for (Connection& connection : node_size->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeArray* out_node_array = dynamic_cast<NodeArray*>(out_node);
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

			if (out_node_array)
			{
				node_size->m_size = out_node_array->m_elements.size() +
					out_node_array->m_other_elements.size();
				node_size->m_val_size.set((int)node_size->m_size);
			}
			else if (out_node_var)
			{
				if (out_node_var->m_value_orig.m_slot == +NODE_SLOT::STRING)
				{
					std::string& str = std::get<std::string>(out_node_var->m_value.data);
					node_size->m_size = str.length();
					node_size->m_val_size.set((int)node_size->m_size);
				}
				else
					NodeColors::set_connection_color(connection, COLOR_TYPE::FAIL);
			}
		}
	}
}
}
