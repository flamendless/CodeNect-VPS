#include "modules/nodes.hpp"

namespace CodeNect
{
std::vector<Node*> Nodes::v_nodes;
Nodes::m_node_t Nodes::m_available_nodes
{
	{
		"Hello", []() -> Node*
		{
			NodeValue value;
			value.set("Hello");

			return new Node(NODE_KIND::VARIABLE, "Hello", value,
				{
					{"String", NODE_SLOT::STRING},
				},
				{
					{"String", NODE_SLOT::STRING},
				}
			);
		}
	},
	{
		"World", []() -> Node*
		{
			NodeValue value;
			value.set("World");

			return new Node(NODE_KIND::VARIABLE, "World", value,
				{
					{"String", NODE_SLOT::STRING},
				},
				{
					{"String", NODE_SLOT::STRING},
				}
			);
		},
	}
};
}
