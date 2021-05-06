#ifndef _NODE_STRING_HPP
#define _NODE_STRING_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeString : public Node
{
	NODE_KIND m_kind = NODE_KIND::STRING;
	NODE_STRING m_string = NODE_STRING::EMPTY;
	NodeValue* m_current_val = nullptr;

	explicit NodeString(
		NODE_STRING str,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeString() {}
};
}

#endif //_NODE_STRING_HPP
