#ifndef _NODE_META_HPP
#define _NODE_META_HPP

#include "modules/node_def.hpp"

namespace CodeNect
{
struct NodeMeta
{
	std::string m_name;
	std::string m_kind;
	std::string m_value;
	std::string m_value_slot;
	std::string m_op;
	std::string m_desc;
	std::vector<std::string> m_input_slots;
	std::vector<std::string> m_output_slots;
	float x;
	float y;

	inline NodeMeta() {}
};

struct ConnectionMeta
{
	std::string m_in_name;
	std::string m_in_slot;

	std::string m_out_name;
	std::string m_out_slot;

	inline ConnectionMeta() {}
};
}

#endif //_NODE_META_HPP
