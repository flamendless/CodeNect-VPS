#include "node/node_string.hpp"

#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeString::NodeString(
	NODE_STRING str,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	PPK_ASSERT(str != +NODE_STRING::EMPTY, "Passsed NODE_STRING should not be EMPTY");

	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(m_kind._to_string());
	m_string = str;
	m_in_slots = in_slots;
	m_out_slots = out_slots;
	PLOGD << "Created NodeString: " << m_name << ", string = " << m_string._to_string();
}
}
