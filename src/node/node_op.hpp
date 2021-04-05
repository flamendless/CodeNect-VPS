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
	bool m_has_valid_connections = false;
	NodeValue* m_current_val = nullptr;

	explicit NodeOperation(
		NODE_OP op,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeOperation() {}
	const char* get_op(void);
};
}

#endif //_NODE_OP_HPP
