#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_cmp.hpp"

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

		node_cmp->m_has_valid_connections = false;

		//store the result
		NodeVariable* res_node_var = nullptr;

		//make preliminary checks
		if (node_cmp->m_connections.size() < 3)
			continue;

		//make sure node_cmp has a connected node_var for the result
		for (const Connection& connection : node_cmp->m_connections)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			Node* out_node = static_cast<Node*>(connection.out_node);
			NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(out_node);
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);

			if (node_cmp && node_var)
			{
				res_node_var = node_var;
				break;
			}
		}

		if (!res_node_var)
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

		//evaluate
		switch (node_cmp->m_cmp)
		{
			case NODE_CMP::EMPTY: break;
			case NODE_CMP::EQ:
			{
				bool res = val_a->is_eq_to(*val_b);
				res_node_var->m_value.set(res);
				break;
			}
			case NODE_CMP::NEQ:
			{
				bool res = val_a->is_neq_to(*val_b);
				res_node_var->m_value.set(res);
				break;
			}
			case NODE_CMP::LT:
			{
				bool res = val_a->is_lt_to(*val_b);
				res_node_var->m_value.set(res);
				break;
			}
			case NODE_CMP::GT:
			{
				bool res = val_a->is_gt_to(*val_b);
				res_node_var->m_value.set(res);
				break;
			}
			case NODE_CMP::LTE:
			{
				bool res = val_a->is_lte_to(*val_b);
				res_node_var->m_value.set(res);
				break;
			}
			case NODE_CMP::GTE:
			{
				bool res = val_a->is_gte_to(*val_b);
				res_node_var->m_value.set(res);
				break;
			}
		}
	}
}
}
