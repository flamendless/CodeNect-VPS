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
	std::vector<bool> m_bool_elements;
	std::vector<int> m_int_elements;
	std::vector<float> m_float_elements;
	std::vector<double> m_double_elements;
	std::vector<std::string> m_string_elements;

	explicit NodeArray(
		NODE_ARRAY array,
		NODE_SLOT slot,
		unsigned int size,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	void set_size(void);
	void add_elements(std::vector<std::string>& v);

	inline ~NodeArray() {}
};
}

#endif //_NODE_ARRAY_HPP
