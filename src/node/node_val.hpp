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
	const std::string get_value_str(void);
	const char* get_spec(void);
};

}

#endif //_NODE_VAL_HPP
