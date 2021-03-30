#include "ui/create_node.hpp"

#include "node/nodes.hpp"
#include "node/node_branch.hpp"

namespace CodeNect
{
void CreateNode::create_node_branch(void)
{
	TempBranchData* branch = std::get<TempBranchData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeBranch* node_branch = static_cast<NodeBranch*>(CreateNode::node_to_edit);
		node_branch->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeBranch: " << node_branch->m_name;
	}
	else
	{
		NodeBranch* node = new NodeBranch();
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}
}
