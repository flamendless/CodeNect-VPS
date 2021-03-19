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
	std::vector<std::string> m_input_slots;
	std::vector<std::string> m_output_slots;
	float x;
	float y;

	inline NodeMeta() {}
};
}

#endif //_NODE_META_HPP
