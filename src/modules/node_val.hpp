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

	void set(bool arg);
	void set(int arg);
	void set(float arg);
	void set(double arg);
	void set(const char* arg);
	void set(NODE_SLOT slot);

	// inline void* get_value()
	// {
	// 	// void* d = node.m_value.get_value();
	// 	// const char* str = static_cast<const char*>(d);
	// 	switch (m_slot)
	// 	{
	// 		case NODE_SLOT::EMPTY: return nullptr; break;
	// 		case NODE_SLOT::BOOL: return &v_bool; break;
	// 		case NODE_SLOT::INTEGER: return &v_int; break;
	// 		case NODE_SLOT::FLOAT: return &v_float; break;
	// 		case NODE_SLOT::DOUBLE: return &v_double; break;
	// 		case NODE_SLOT::STRING: return (void*)v_string; break;
	// 	}
	// }

	const std::string get_value_str(void);
	const char* get_spec(void);
	void draw(void);
};

}

#endif //_NODE_VAL_HPP
