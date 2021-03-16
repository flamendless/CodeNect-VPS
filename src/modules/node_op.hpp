#ifndef _NODE_OP_HPP
#define _NODE_OP_HPP

#include "modules/node_def.hpp"
#include "modules/node.hpp"

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
		Node::m_kind = m_kind;
		Node::m_name = name;
		m_in_slots = in_slots;
		m_out_slots = out_slots;
	}

	inline ~NodeOp() {}

	inline virtual void on_connect(Node* in, Node* out) override
	{
	}

	inline void draw(void) override {}
};
}

#endif //_NODE_OP_HPP
