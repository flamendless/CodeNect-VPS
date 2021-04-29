#include "node/node_array_access.hpp"

#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeArrayAccess::NodeArrayAccess(
	unsigned int index,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	NodeGet::m_get = m_get;
	Node::m_kind = NodeGet::m_kind;
	Node::m_name = Nodes::get_id(m_get._to_string());

	m_index_orig = index;
	m_index = index;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	PPK_ASSERT(NodeGet::m_get != +NODE_GET::EMPTY, "NODE_GET must not be EMPTY");
	PLOGD << "Created NodeGet: " << m_name << ", " << Node::m_kind._to_string();
}
}
