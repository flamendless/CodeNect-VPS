#ifndef _NODE_OP_HPP
#define _NODE_OP_HPP

#include "modules/node_def.hpp"
#include "modules/node.hpp"

namespace CodeNect
{
struct NodeOp : public Node
{
	NODE_OP m_op = NODE_OP::EMPTY;
	NODE_KIND m_kind = NODE_KIND::OPERATION;
	const char* m_icon;

	explicit NodeOp(
		NODE_OP op,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeOp() {}
	virtual void on_connect(Node* in, Node* out) override;
	virtual void draw(void) override;
};
}

#endif //_NODE_OP_HPP
