#include "node/node_math.hpp"

#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeMath::NodeMath(
	NODE_MATH math,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	PPK_ASSERT(math != +NODE_MATH::EMPTY, "Passsed NODE_MATH should not be EMPTY");

	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(m_kind._to_string());
	m_math = math;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	switch (math)
	{
		case NODE_MATH::EMPTY: break;
		case NODE_MATH::ROOT: m_data = MathRoot(); break;
	}

	PLOGD << "Created NodeMath: " << m_name << ", math = " << m_math._to_string();
}
}
