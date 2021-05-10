#include "node/node_prompt.hpp"

#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodePrompt::NodePrompt(
	std::string str,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	NodeAction::m_action = m_action;
	Node::m_kind = NodeAction::m_kind;
	Node::m_name = Nodes::get_id(m_action._to_string());

	m_orig_str = str;
	m_in_slots = in_slots;
	m_out_slots = out_slots;
	m_fake_input.set((std::string)"");

	PPK_ASSERT(NodeAction::m_action != +NODE_ACTION::EMPTY, "NODE_ACTION must not be EMPTY");
	PLOGD << "Created NodeAction: " << m_name << ", " << Node::m_kind._to_string();
}
}
