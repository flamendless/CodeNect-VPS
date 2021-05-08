#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_for.hpp"
#include "node/node_var.hpp"

namespace CodeNect::NodeLogic
{
bool validate_node_for(Node* in_node, Node* out_node)
{
	NodeFor* node_for = dynamic_cast<NodeFor*>(out_node);
	NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
	const char* out_it = node_for->m_out_slots[0].title;
	bool same = std::strcmp(out_it, "INTEGER - iterator") == 0;

	if (!(node_for && node_var && same))
	{
		Alert::open_for_docs("Iterator can only be connected to Variable Node", DOC_ID::IT_MUST_BE_VAR);
		return false;
	}

	return true;
}
}
