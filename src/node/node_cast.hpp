#ifndef _NODE_CAST_HPP
#define _NODE_CAST_HPP

#include "node/node.hpp"
#include "node/node_def.hpp"

namespace CodeNect
{
struct NodeCast : public Node
{
	NODE_KIND m_kind = NODE_KIND::CAST;

	explicit NodeCast(
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeCast() {}
};
}

#endif //_NODE_CAST_HPP
