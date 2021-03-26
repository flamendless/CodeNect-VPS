#include "node/node_cmp.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeComparison::NodeComparison(
	NODE_CMP cmp,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	const unsigned int id = ++Nodes::m_ids["COMPARISON"];
	Node::m_kind = m_kind;

	std::string str_cmp = m_kind._to_string();
	std::string* str_id = new std::string(str_cmp + "_" + std::to_string(id));
	Node::m_name = str_id->c_str();

	m_cmp = cmp;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	PLOGD << "Created NodeComparison: " << m_name << ", " << m_cmp._to_string();
}
}
