#include "node/node_entry.hpp"

#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeEntry::NodeEntry(
	const v_slot_info_t&& out_slots
)
{
	Node::m_kind = m_kind;
	Node::m_name = "ENTRY";
	m_out_slots = out_slots;

	PLOGD << "Created NodeEntry: " << m_name;
}
}
