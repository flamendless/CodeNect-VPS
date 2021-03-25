#include "node/node_var.hpp"

#include "core/config.hpp"

namespace CodeNect
{
NodeVariable::NodeVariable(
	const char* name,
	NodeValue value,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	Node::m_kind = m_kind;
	Node::m_name = name;
	Node::m_in_slots = in_slots;
	Node::m_out_slots = out_slots;

	m_value_orig.copy(value);
	m_value.copy(value);

	PLOGD << "Created NodeVariable: " << m_name << ", value = " << m_value_orig.get_value_str();
}
}
