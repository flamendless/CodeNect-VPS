#ifndef _NODE_VAL_HPP
#define _NODE_VAL_HPP

#include "modules/node_def.hpp"

namespace CodeNect
{
struct NodeValue
{
	NODE_SLOT m_slot = NODE_SLOT::EMPTY;

	union
	{
		bool v_bool;
		int v_int;
		float v_float;
		double v_double;
		const char* v_string;
	};

	NodeValue operator+(NodeValue& rhs);

	void set(bool arg);
	void set(int arg);
	void set(float arg);
	void set(double arg);
	void set(const char* arg);
	void copy(NODE_SLOT& slot);
	void to_bool(const char* str);
	void to_int(const char* str);
	void to_float(const char* str);
	void to_double(const char* str);

	void* get_value();
	const std::string get_value_str(void);
	const char* get_spec(void);
	void draw(void);
};

}

#endif //_NODE_VAL_HPP
