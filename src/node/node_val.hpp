#ifndef _NODE_VAL_HPP
#define _NODE_VAL_HPP

#include <variant>
#include "node/node_def.hpp"

namespace CodeNect
{
struct NodeValue
{
	NODE_SLOT m_slot = NODE_SLOT::EMPTY;

	std::variant<bool, int, float, double, std::string> data;

	void add(NodeValue& to_add);
	void sub(NodeValue& to_sub);
	void mul(NodeValue& to_mul);
	void div(NodeValue& to_div);
	void mod(NodeValue& to_div);

	bool cast_from(NodeValue& from_val);

	bool is_eq_to(NodeValue& other);
	bool is_neq_to(NodeValue& other);
	bool is_lt_to(NodeValue& other);
	bool is_gt_to(NodeValue& other);
	bool is_lte_to(NodeValue& other);
	bool is_gte_to(NodeValue& other);
	bool is_or_to(NodeValue& other);
	bool is_and_to(NodeValue& other);

	void set(bool arg);
	void set(int arg);
	void set(float arg);
	void set(double arg);
	void set(std::string arg);
	void copy(NodeValue& val);
	void copy(NODE_SLOT& slot);
	void to_bool(const char* str);
	void to_int(const char* str);
	void to_float(const char* str);
	void to_double(const char* str);

	void* get_value();
	const std::string get_type_str(void);
	const std::string get_value_str(void);
	const std::string get_value_str_ex(void);
	const char* get_spec(void);
};

}

#endif //_NODE_VAL_HPP
