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

	NodeDS::m_ds = m_ds;
	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(array._to_string());
	m_array = array;
	m_fixed_size = size;
	m_slot = slot;
	m_in_slots = in_slots;
	m_out_slots = out_slots;
	m_elements.reserve(m_fixed_size);

	PLOGD << "Created NodeArray: " << m_name << ", array = " << m_array._to_string()
		<< ", fixed size = " << m_fixed_size;
}

void NodeArray::add_elements(std::vector<std::string>& v)
{
	for (std::vector<std::string>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		std::string str = *it;
		NodeValue* val = new NodeValue();

		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: break;
			case NODE_SLOT::BOOL:
			{
				val->to_bool(str.c_str());
				break;
			}
			case NODE_SLOT::INTEGER:
			{
				val->to_int(str.c_str());
				break;
			}
			case NODE_SLOT::FLOAT:
			{
				val->to_float(str.c_str());
				break;
			}
			case NODE_SLOT::DOUBLE:
			{
				val->to_double(str.c_str());
				break;
			}
			case NODE_SLOT::STRING:
			{
				val->set(str);
				break;
			}
		}

		m_elements.push_back(val);
	}
}

void NodeArray::elements_to_vec_str(std::vector<std::string>& v)
{
	for (std::vector<NodeValue*>::iterator it = m_elements.begin();
		it != m_elements.end();
		it++)
	{
		NodeValue* val = *it;
		std::string str = val->get_value_str();
		v.push_back(str);
	}
}
}
