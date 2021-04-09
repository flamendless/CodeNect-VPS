#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"
#include "node/node_array_access.hpp"

namespace CodeNect::NodeLogic
{
void process_cmp(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(*it);

		if (!node_cmp)
			continue;

		node_cmp->current_res = false;
		node_cmp->m_has_valid_connections = false;

		//store the result
		NodeVariable* res_var = nullptr;
		NodeBranch* res_branch = nullptr;

		//make preliminary checks
		if (node_cmp->m_connections.size() < 3)
			continue;

		//make sure node_cmp has a connected node_var or node_branch for the result (rhs)
		for (const Connection& connection : node_cmp->m_connections)
		{
			if (res_var || res_branch)
				break;

			Node* in_node = static_cast<Node*>(connection.in_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(in_node);

			if (node_var)
			{
				res_var = node_var;
				break;
			}

			if (node_branch)
			{
				res_branch = node_branch;
				break;
			}
		}

		if (!res_var && !res_branch)
			break;

		//for storing the input (lhs) nodes to be compared
		std::vector<NodeValue*> v_values;

		for (const Connection& connection : node_cmp->m_connections)
		{
			if (node_cmp->m_in_slots[0].kind == +NODE_SLOT::BOOL)
			{
				if (v_values.size() == 2)
					break;
			}

			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);
			NodeArrayAccess* node_array_access = dynamic_cast<NodeArrayAccess*>(out_node);

			if (node_var)
				v_values.push_back(&node_var->m_value);
			else if (node_math)
				v_values.push_back(node_math->m_current_val);
			else if (node_array_access)
				v_values.push_back(node_array_access->m_current_val);
		}

		if (v_values.size() < 2)
			continue;

		node_cmp->m_has_valid_connections = true;
		NodeValue result;
		bool res = false;
		bool is_first = true;

		for (NodeValue* value : v_values)
		{
			if (is_first)
			{
				result.copy(*value);
				is_first = false;
				continue;
			}

			//evaluate
			switch (node_cmp->m_cmp)
			{
				case NODE_CMP::EMPTY: break;
				case NODE_CMP::EQ:
				{
					res = result.is_eq_to(*value);
					break;
				}
				case NODE_CMP::NEQ:
				{
					res = result.is_neq_to(*value);
					break;
				}
				case NODE_CMP::LT:
				{
					res = result.is_lt_to(*value);
					break;
				}
				case NODE_CMP::GT:
				{
					res = result.is_gt_to(*value);
					break;
				}
				case NODE_CMP::LTE:
				{
					res = result.is_lte_to(*value);
					break;
				}
				case NODE_CMP::GTE:
				{
					res = result.is_gte_to(*value);
					break;
				}
				case NODE_CMP::OR:
				{
					res = result.is_or_to(*value);
					break;
				}
				case NODE_CMP::AND:
				{
					res = result.is_and_to(*value);
					break;
				}
			}
		}

		if (res_var)
			res_var->m_value.set(res);

		node_cmp->current_res = res;
	}
}
}
