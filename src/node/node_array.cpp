#include "node/node_array.hpp"

#include "ppk_assert.h"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeArray::NodeArray(
	NODE_ARRAY array,
	NODE_SLOT slot,
	unsigned int size,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	PPK_ASSERT(array != +NODE_ARRAY::EMPTY, "Passsed NODE_ARRAY should not be EMPTY");
	PPK_ASSERT(slot != +NODE_SLOT::EMPTY, "Passsed NODE_SLOT should not be EMPTY");

	if (array == +NODE_ARRAY::FIXED)
		PPK_ASSERT(size != 0, "size for fixed array should not be zero");
	else if (array == +NODE_ARRAY::DYNAMIC)
		PPK_ASSERT(size == 0, "size for dynamic array should be zero");

	NodeDS::m_ds = m_ds;
	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(array._to_string());
	m_array = array;
	m_size = size;
	m_slot = slot;
	m_in_slots = in_slots;
	m_out_slots = out_slots;
	set_size();

	PLOGD << "Created NodeArray: " << m_name << ", array = " << m_array._to_string()
		<< ", size = " << m_size;
}

void NodeArray::set_size(void)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL:
		{
			std::vector<bool> vec;
			if (m_size != 0) vec.reserve(m_size);
			m_vec = vec;
			break;
		}
		case NODE_SLOT::INTEGER:
		{
			std::vector<int> vec;
			if (m_size != 0) vec.reserve(m_size);
			m_vec = vec;
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			std::vector<float> vec;
			if (m_size != 0) vec.reserve(m_size);
			m_vec = vec;
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			std::vector<double> vec;
			if (m_size != 0) vec.reserve(m_size);
			m_vec = vec;
			break;
		}
		case NODE_SLOT::STRING:
		{
			std::vector<std::string> vec;
			if (m_size != 0) vec.reserve(m_size);
			m_vec = vec;
			break;
		}
	}
}
}
