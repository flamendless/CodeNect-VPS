#ifndef _NODE_ENTRY_HPP
#define _NODE_ENTRY_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeEntry : public Node
{
	NODE_KIND m_kind = NODE_KIND::ENTRY;

	explicit NodeEntry(
		const v_slot_info_t&& out_slots
	);

	inline ~NodeEntry() {}
};
}

#endif //_NODE_ENTRY_HPP
