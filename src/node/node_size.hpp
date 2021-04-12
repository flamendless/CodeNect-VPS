#ifndef _NODE_SIZE_HPP
#define _NODE_SIZE_HPP

#include "node/node_get.hpp"

namespace CodeNect
{
struct NodeSize : public NodeGet
{
	NODE_GET m_get = NODE_GET::SIZE;

	unsigned int m_size;

	explicit NodeSize(
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeSize() {}
};
}

#endif //_NODE_SIZE_HPP
