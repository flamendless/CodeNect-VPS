#include "node/node_branch.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeBranch::NodeBranch()
{
	const unsigned int id = ++Nodes::m_ids["BRANCH"];
	Node::m_kind = m_kind;

	std::string str_branch = m_kind._to_string();
	std::string* str_id = new std::string(str_branch + "_" + std::to_string(id));
	Node::m_name = str_id->c_str();

	NODE_SLOT slot_in = NODE_SLOT::BOOL;
	NODE_SLOT b_true = NODE_SLOT::BOOL;
	NODE_SLOT b_false = NODE_SLOT::BOOL;

	m_in_slots = {{slot_in._to_string(), slot_in}};
	m_out_slots = {
		{"TRUE", b_true},
		{"FALSE", b_false},
	};

	PLOGD << "Created NodeBranch: " << m_name;
}
}
