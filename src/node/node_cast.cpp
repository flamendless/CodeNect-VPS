#include "node/node_cast.hpp"

#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeCast::NodeCast(
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	Nodes::cast_id++;

	Node::m_kind = m_kind;

	std::string str_cast = m_kind._to_string();
	std::string* str_id = new std::string(str_cast + "_" + std::to_string(Nodes::cast_id));
	Node::m_name = str_id->c_str();

	m_in_slots = in_slots;
	m_out_slots = out_slots;

	PLOGD << "Created NodeCast: " << m_name;
}
}
