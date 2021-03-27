#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"

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

		//make sure node_cmp has a connected node_var or node_branch for the result
		for (const Connection& connection : node_cmp->m_connections)
		{
			if (res_var || res_branch)
				break;

			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(out_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(in_node);

			if (node_cmp && node_var)
			{
				res_var = node_var;
				break;
			}

			if (node_cmp && node_branch)
			{
				res_branch = node_branch;
				break;
			}
		}

		if (!res_var && !res_branch)
			break;

		NodeVariable* node_var_a = nullptr;
		NodeVariable* node_var_b = nullptr;

		//get the two node_vars to be compared
		for (const Connection& connection : node_cmp->m_connections)
		{
			if (node_var_a && node_var_b)
				break;

			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(in_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);

			if (node_cmp && node_var)
			{
				if (!node_var_a)
				{
					node_var_a = node_var;
					continue;
				}

				if (!node_var_b)
				{
					node_var_b = node_var;
					continue;
				}
			}
		}

		if (!node_var_a || !node_var_b)
			continue;

		node_cmp->m_has_valid_connections = true;
		NodeValue* val_a = &node_var_a->m_value;
		NodeValue* val_b = &node_var_b->m_value;
		bool res = false;

		//evaluate
		switch (node_cmp->m_cmp)
		{
			case NODE_CMP::EMPTY: break;
			case NODE_CMP::EQ:
			{
				res = val_a->is_eq_to(*val_b);
				break;
			}
			case NODE_CMP::NEQ:
			{
				res = val_a->is_neq_to(*val_b);
				break;
			}
			case NODE_CMP::LT:
			{
				res = val_a->is_lt_to(*val_b);
				break;
			}
			case NODE_CMP::GT:
			{
				res = val_a->is_gt_to(*val_b);
				break;
			}
			case NODE_CMP::LTE:
			{
				res = val_a->is_lte_to(*val_b);
				break;
			}
			case NODE_CMP::GTE:
			{
				res = val_a->is_gte_to(*val_b);
				break;
			}
		}

		if (res_var)
			res_var->m_value.set(res);

		node_cmp->current_res = res;
	}
}
}
