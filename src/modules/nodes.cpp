#include "modules/nodes.hpp"
#include "modules/node_val.hpp"
#include "modules/node_var.hpp"

namespace CodeNect
{
std::vector<Node*> Nodes::v_nodes;
Nodes::m_node_t Nodes::m_available_nodes
{
	{
		"Test_Int", []() -> Node*
		{
			NodeValue value;
			value.set(11);

			return new NodeVariable("a", value,
				{
					{"INTEGER", NODE_SLOT::INTEGER},
				},
				{
					{"INTEGER", NODE_SLOT::INTEGER},
				}
			);
		}
	},
	{
		"Test_Int2", []() -> Node*
		{
			NodeValue value;
			value.set(15);

			return new NodeVariable("b", value,
				{
					{"INTEGER", NODE_SLOT::INTEGER},
				},
				{
					{"INTEGER", NODE_SLOT::INTEGER},
				}
			);
		}
	},
};
}
