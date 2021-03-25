#ifndef _NODE_IF_HPP
#define _NODE_IF_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeIf : public Node
{
	NODE_KIND m_kind = NODE_KIND::IF;

	inline explicit NodeIf(
		const char* name,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	)
	{
		m_name = name;
		m_in_slots = in_slots;
		m_out_slots = out_slots;
	}

	inline ~NodeIf() {}
	inline void draw_node(void) override {}
};
}

#endif //_NODE_IF_HPP
