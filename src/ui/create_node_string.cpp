#include "ui/create_node.hpp"

#include "node/node.hpp"
#include "node/nodes.hpp"
#include "node/node_string.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
void CreateNode::create_node_string(void)
{
	TempStringData* tmp = std::get<TempStringData*>(data);

	if (CreateNode::is_edit_mode)
	{
		NodeString* node_math = static_cast<NodeString*>(CreateNode::node_to_edit);
		node_math->m_desc = CreateNode::buf_desc;
		PLOGD << "Edited NodeString: " << node_math->m_name;
	}
	else
	{
		v_slot_info_t&& in = {};
		v_slot_info_t&& out = {};

		NODE_SLOT slot_in = NODE_SLOT::STRING;
		NODE_SLOT slot_out = NODE_SLOT::STRING;
		in.push_back({slot_in._to_string(), slot_in});
		out.push_back({slot_out._to_string(), slot_out});

		NodeString* node = new NodeString(CreateNode::str, std::move(in), std::move(out));
		node->set_desc(CreateNode::buf_desc);
		Nodes::v_nodes.push_back(node);
		ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	}
}
}
