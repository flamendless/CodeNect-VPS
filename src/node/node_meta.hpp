#ifndef _NODE_META_HPP
#define _NODE_META_HPP

#include <string>
#include "node/node_def.hpp"

namespace CodeNect
{
struct NodeMeta
{
	std::string m_name;
	std::string m_kind;
	std::string m_desc;
	std::vector<std::string> m_input_slots;
	std::vector<std::string> m_output_slots;
	float x;
	float y;

	//these are based depending on the kind of node
	//NodeVariable
	std::string m_value;
	std::string m_value_slot;

	//NodeOperation
	std::string m_op;

	//NodeComparison
	std::string m_cmp_str;
	std::string m_cmp;

	//NodePrint
	std::string m_action;
	std::string m_orig_str;
	std::string m_override;
	std::string m_append_newline;

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
