#ifndef _NODE_ARRAY_ACCESS_HPP
#define _NODE_ARRAY_ACCESS_HPP

#include "node/node_get.hpp"

namespace CodeNect
{
struct NodeArrayAccess : public NodeGet
{
	NODE_GET m_get = NODE_GET::ARRAY_ACCESS;

	unsigned int m_index = 0;
	unsigned int m_index_orig = 0;
	NodeValue* m_current_val = nullptr;
	bool m_has_array = false;

	explicit NodeArrayAccess(
		unsigned int index,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeArrayAccess() {}
};
}

#endif //_NODE_ARRAY_ACCESS_HPP
