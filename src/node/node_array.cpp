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
			if (m_size != 0) m_bool_elements.reserve(m_size);
			break;
		}
		case NODE_SLOT::INTEGER:
		{
			if (m_size != 0) m_int_elements.reserve(m_size);
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			if (m_size != 0) m_float_elements.reserve(m_size);
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			if (m_size != 0) m_double_elements.reserve(m_size);
			break;
		}
		case NODE_SLOT::STRING:
		{
			if (m_size != 0) m_string_elements.reserve(m_size);
			break;
		}
	}
}

void NodeArray::add_elements(std::vector<std::string>& v)
{
	for (std::vector<std::string>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		std::string str = *it;

		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: break;
			case NODE_SLOT::BOOL:
			{
				bool b;
				if (str.compare("true") == 0)
					b = true;
				else if (str.compare("false") == 0)
					b = false;
				m_bool_elements.push_back(b);
				break;
			}
			case NODE_SLOT::INTEGER:
			{
				m_int_elements.push_back(std::stoi(str));
				break;
			}
			case NODE_SLOT::FLOAT:
			{
				m_float_elements.push_back(std::stof(str));
				break;
			}
			case NODE_SLOT::DOUBLE:
			{
				m_double_elements.push_back(std::stod(str));
				break;
			}
			case NODE_SLOT::STRING:
			{
				m_string_elements.push_back(str);
				break;
			}
		}
	}

	m_size = v.size();
}

void NodeArray::elements_to_vec_str(std::vector<std::string>& v)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL:
		{
			for (int i = 0; i < m_bool_elements.size(); i++)
			{
				bool b = m_bool_elements[i];
				v.push_back(b ? "true" : "false");
			}
			break;
		}
		case NODE_SLOT::INTEGER:
		{
			for (int i = 0; i < m_int_elements.size(); i++)
			{
				v.push_back(std::to_string(m_int_elements[i]));
			}
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			for (int i = 0; i < m_float_elements.size(); i++)
			{
				v.push_back(std::to_string(m_float_elements[i]));
			}
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			for (int i = 0; i < m_double_elements.size(); i++)
			{
				v.push_back(std::to_string(m_double_elements[i]));
			}
			break;
		}
		case NODE_SLOT::STRING:
		{
			for (int i = 0; i < m_string_elements.size(); i++)
			{
				v.push_back(m_string_elements[i]);
			}
			break;
		}
	}
}

void NodeArray::clear_elements(void)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL:
		{
			m_bool_elements.clear();
			break;
		}
		case NODE_SLOT::INTEGER:
		{
			m_int_elements.clear();
			break;
		}
		case NODE_SLOT::FLOAT:
		{
			m_float_elements.clear();
			break;
		}
		case NODE_SLOT::DOUBLE:
		{
			m_double_elements.clear();
			break;
		}
		case NODE_SLOT::STRING:
		{
			m_string_elements.clear();
			break;
		}
	}
}
}
