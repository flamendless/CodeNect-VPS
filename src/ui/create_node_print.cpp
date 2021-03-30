#include "ui/create_node.hpp"

#include "node/nodes.hpp"
#include "node/node_print.hpp"

namespace CodeNect
{
void CreateNode::create_node_print(void)
{
	TempPrintData* print = std::get<TempPrintData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodePrint* node_print = static_cast<NodePrint*>(CreateNode::node_to_edit);
		node_print->m_desc = print->buf_desc;
		PLOGD << "Edited NodePrint: " << node_print->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		// in.push_back({print->slot._to_string(), print->slot});

		NodePrint* node = new NodePrint(print->buf_str, std::move(in), std::move(out));
		node->m_desc = print->buf_desc;
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}

void CreateNode::draw_print(void)
{

}
}
