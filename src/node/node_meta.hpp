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

	//NodePrint/NodePrompt/NodeSet
	std::string m_action;
	std::string m_orig_str;
	std::string m_override;
	std::string m_append;
	std::string m_append_newline;
	std::string m_fake_input;
	std::string m_target_var_name;
	std::string m_set_value;

	//NodeMath
	std::string m_math;

	//NodeDS
	std::string m_ds;
	//NodeDS::ARRAY
	std::string m_array;
	std::string m_array_size;
	std::string m_array_slot;
	std::vector<std::string> m_array_elements;

	//NodeGet
	std::string m_get;
	//NodeArrayAccess
	std::string m_index;
	//NodeSize
	std::string m_size;

	//NodeString
	std::string m_string;

	//NodeLoop
	std::string m_loop;
	std::string m_start_index;
	std::string m_end_index;
	std::string m_increment;
	std::string m_iterator_name;
	std::string m_loop_cmp;
	std::string m_loop_in;
	std::string m_loop_out;

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
