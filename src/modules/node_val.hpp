#ifndef _NODE_VAL_HPP
#define _NODE_VAL_HPP

#include "modules/node_def.hpp"
#include "imgui.h"

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

	inline void set(bool arg){ v_bool = arg; m_slot = NODE_SLOT::BOOL; }
	inline void set(int arg){ v_int = arg; m_slot = NODE_SLOT::INTEGER; }
	inline void set(float arg){ v_float = arg; m_slot = NODE_SLOT::FLOAT; }
	inline void set(double arg){ v_double = arg; m_slot = NODE_SLOT::DOUBLE; }
	inline void set(const char* arg){ v_string = arg; m_slot = NODE_SLOT::STRING; }
	inline void set(NODE_SLOT slot)
	{
		switch (slot)
		{
			case NODE_SLOT::EMPTY: break;
			case NODE_SLOT::BOOL: set(true); break;
			case NODE_SLOT::INTEGER: set(0); break;
			case NODE_SLOT::FLOAT: set(0); break;
			case NODE_SLOT::DOUBLE: set(0); break;
			case NODE_SLOT::STRING: set(""); break;
		}
	}

	inline void* get_value()
	{
		// void* d = node.m_value.get_value();
		// const char* str = static_cast<const char*>(d);
		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: return nullptr; break;
			case NODE_SLOT::BOOL: return &v_bool; break;
			case NODE_SLOT::INTEGER: return &v_int; break;
			case NODE_SLOT::FLOAT: return &v_float; break;
			case NODE_SLOT::DOUBLE: return &v_double; break;
			case NODE_SLOT::STRING: return (void*)v_string; break;
		}
	}

	inline const char* get_spec()
	{
		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: return nullptr; break;
			case NODE_SLOT::BOOL: return "%d"; break;
			case NODE_SLOT::INTEGER: return "%d"; break;
			case NODE_SLOT::FLOAT: return "%f"; break;
			case NODE_SLOT::DOUBLE: return "%lf"; break;
			case NODE_SLOT::STRING: return "%s"; break;
		}
	}

	inline void draw()
	{
		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: ImGui::Text("Empty?! (this should be an error)"); break;
			case NODE_SLOT::BOOL: ImGui::Text("%s", v_bool ? "true" : "false"); break;
			case NODE_SLOT::INTEGER: ImGui::Text("%d", v_int); break;
			case NODE_SLOT::FLOAT: ImGui::Text("%f", v_float); break;
			case NODE_SLOT::DOUBLE: ImGui::Text("%.8lf", v_double); break;
			case NODE_SLOT::STRING: ImGui::Text("%s", v_string); break;
		}
	}
};

}

#endif //_NODE_VAL_HPP
