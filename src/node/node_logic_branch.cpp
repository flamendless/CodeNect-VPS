#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_branch(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeBranch* node_branch = dynamic_cast<NodeBranch*>(*it);

		if (!node_branch)
			continue;

		if (node_branch->m_connections.size() < 2)
			continue;

		//store the results
		bool res = false;
		std::vector<Connection*> v_c_true;
		std::vector<Connection*> v_c_false;

		//check if there is a node_var for result
		for (Connection& connection : node_branch->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(out_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);

			if (node_branch && node_var)
			{
				const char* slot = connection.out_slot;

				if (std::strcmp(slot, "TRUE") == 0)
				{
					v_c_true.push_back(&connection);
					continue;
				}

				if (std::strcmp(slot, "FALSE") == 0)
				{
					v_c_false.push_back(&connection);
					continue;
				}
			}
		}

		//check if there's a node_cmp or node_var with (bool) lhs
		for (const Connection& connection : node_branch->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(out_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(in_node);

			//check if lhs is node_cmp
			if (node_cmp && node_branch)
			{
				res = node_cmp->current_res;
				break;
			}

			//check if lhs is node_var
			if (node_var && node_branch)
			{
				res = std::get<bool>(node_var->m_value.data);
				break;
			}
		}

		if (res)
		{
			for (Connection* c_true : v_c_true)
			{
				c_true->is_valid = true;
				NodeColors::set_connection_color(*c_true, COLOR_TYPE::TRUE);
			}

			for (Connection* c_false : v_c_false)
			{
				c_false->is_valid = false;
				NodeColors::set_connection_color(*c_false, COLOR_TYPE::FALSE);
			}
		}
		else
		{
			for (Connection* c_true : v_c_true)
			{
				c_true->is_valid = false;
				NodeColors::set_connection_color(*c_true, COLOR_TYPE::FALSE);
			}

			for (Connection* c_false : v_c_false)
			{
				c_false->is_valid = true;
				NodeColors::set_connection_color(*c_false, COLOR_TYPE::TRUE);
			}
		}
	}
}
}
