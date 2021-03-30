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
	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(m_kind._to_string());

	m_in_slots = in_slots;
	m_out_slots = out_slots;

	PLOGD << "Created NodeCast: " << m_name;
}
}
