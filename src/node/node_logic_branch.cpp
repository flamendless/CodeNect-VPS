#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_branch.hpp"

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
	}
}
}
