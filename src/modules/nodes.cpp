#include "modules/nodes.hpp"

namespace CodeNect
{
std::vector<Node*> Nodes::v_nodes;
Nodes::m_node_t Nodes::m_available_nodes
{
	{
		"Hello", []() -> Node*
		{
			return new Node("Hello",
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
			return new Node("World",
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