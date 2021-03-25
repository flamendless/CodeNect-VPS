#ifndef _NODE_OP_HPP
#define _NODE_OP_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeOperation : public Node
{
	NODE_OP m_op = NODE_OP::EMPTY;
	NODE_KIND m_kind = NODE_KIND::OPERATION;
	const char* m_icon;
	bool has_valid_connections = false;

	explicit NodeOperation(
		NODE_OP op,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeOperation() {}
};
}

#endif //_NODE_OP_HPP
