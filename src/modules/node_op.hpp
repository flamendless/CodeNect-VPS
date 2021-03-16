#ifndef _NODE_OP_HPP
#define _NODE_OP_HPP

#include "modules/node_def.hpp"
#include "modules/node.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
struct NodeOp : public Node
{
	NODE_OP op = NODE_OP::EMPTY;
	NODE_KIND m_kind = NODE_KIND::OPERATION;

	inline explicit NodeOp(
		const char* name,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	)
	{
		m_str_kind = m_kind._to_string();
		m_name = name;
		m_in_slots = in_slots;
		m_out_slots = out_slots;
	}

	inline ~NodeOp() {}

	inline void draw_node(void) override {}
};
}

#endif //_NODE_OP_HPP
