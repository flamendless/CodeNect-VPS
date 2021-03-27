#include "ui/create_node.hpp"

#include "node/nodes.hpp"
#include "node/node_branch.hpp"

namespace CodeNect
{
void CreateNode::create_branch_node(void)
{
	if (CreateNode::is_edit_mode)
	{
		TempBranchData* branch = std::get<TempBranchData*>(data);
		NodeBranch* node_branch = static_cast<NodeBranch*>(CreateNode::node_to_edit);

		node_branch->m_desc = branch->buf_desc;

		PLOGD << "Edited NodeBranch: " << node_branch->m_name;
	}
	else
	{
		TempBranchData* branch = std::get<TempBranchData*>(data);

		NodeBranch* node = new NodeBranch();
		node->m_desc = branch->buf_desc;

		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}
}
