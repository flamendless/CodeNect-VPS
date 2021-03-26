#ifndef _NODE_IF_COMPARISON
#define _NODE_IF_COMPARISON

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeComparison : public Node
{
	NODE_KIND m_kind = NODE_KIND::COMPARISON;
	NODE_CMP m_cmp = NODE_CMP::EMPTY;

	explicit NodeComparison(
		NODE_CMP cmp,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeComparison() {}
};
}

#endif //_NODE_IF_COMPARISON
