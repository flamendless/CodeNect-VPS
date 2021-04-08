#ifndef _NODE_ARRAY_ACCESS_HPP
#define _NODE_ARRAY_ACCESS_HPP

#include "node/node_action.hpp"

namespace CodeNect
{
struct NodeArrayAccess : public NodeAction
{
	NODE_ACTION m_action = NODE_ACTION::ARRAY_ACCESS;

	unsigned int m_index = 0;
	NodeValue* m_current_val = nullptr;

	explicit NodeArrayAccess(
		unsigned int index,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeArrayAccess() {}
};
}

#endif //_NODE_ARRAY_ACCESS_HPP
