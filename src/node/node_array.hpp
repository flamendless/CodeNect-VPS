#ifndef _NODE_ARRAY_HPP
#define _NODE_ARRAY_HPP

#include <variant>
#include "node/node_def.hpp"
#include "node/node_val.hpp"
#include "node/node_ds.hpp"

namespace CodeNect
{
struct NodeArray : public NodeDS
{
	NODE_DS m_ds = NODE_DS::ARRAY;
	NODE_ARRAY m_array = NODE_ARRAY::EMPTY;
	NODE_SLOT m_slot = NODE_SLOT::EMPTY;

	unsigned int m_fixed_size = 0;
	std::vector<NodeValue*> m_elements;
	std::vector<NodeValue*> m_other_elements;
	std::vector<NodeValue*> m_from_string;

	explicit NodeArray(
		NODE_ARRAY array,
		NODE_SLOT slot,
		unsigned int size,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	void add_elements(std::vector<std::string>& v);
	void elements_to_vec_str(std::vector<std::string>& v);

	inline ~NodeArray() {}
};
}

#endif //_NODE_ARRAY_HPP
