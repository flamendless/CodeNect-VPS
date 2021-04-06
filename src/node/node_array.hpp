#ifndef _NODE_ARRAY_HPP
#define _NODE_ARRAY_HPP

#include <variant>
#include "node/node_def.hpp"
#include "node/node_ds.hpp"

namespace CodeNect
{
struct NodeArray : public NodeDS
{
	NODE_DS m_ds = NODE_DS::ARRAY;
	NODE_ARRAY m_array = NODE_ARRAY::EMPTY;
	NODE_SLOT m_slot = NODE_SLOT::EMPTY;

	unsigned int m_size = 0;
	std::variant<
		std::vector<bool>,
		std::vector<int>,
		std::vector<float>,
		std::vector<double>,
		std::vector<std::string>
	>m_vec;

	explicit NodeArray(
		NODE_ARRAY array,
		NODE_SLOT slot,
		unsigned int size,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	void set_size(void);

	inline ~NodeArray() {}
};
}

#endif //_NODE_ARRAY_HPP
