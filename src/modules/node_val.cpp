#include "modules/node_val.hpp"

#include "imgui.h"

namespace CodeNect
{
void NodeValue::set(bool arg){ v_bool = arg; m_slot = NODE_SLOT::BOOL; }
void NodeValue::set(int arg){ v_int = arg; m_slot = NODE_SLOT::INTEGER; }
void NodeValue::set(float arg){ v_float = arg; m_slot = NODE_SLOT::FLOAT; }
void NodeValue::set(double arg){ v_double = arg; m_slot = NODE_SLOT::DOUBLE; }
void NodeValue::set(const char* arg){ v_string = arg; m_slot = NODE_SLOT::STRING; }
void NodeValue::set(NODE_SLOT slot)
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

const std::string NodeValue::get_value_str(void)
{
	switch (m_slot)
	{
		case NODE_SLOT::EMPTY: return "empty"; break;
		case NODE_SLOT::BOOL: return v_bool ? "true" : "false"; break;
		case NODE_SLOT::INTEGER: return std::to_string(v_int); break;
		case NODE_SLOT::FLOAT: return std::to_string(v_float); break;
		case NODE_SLOT::DOUBLE: return std::to_string(v_double); break;
		case NODE_SLOT::STRING: return v_string; break;
	}
}

const char* NodeValue::get_spec(void)
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

void NodeValue::draw(void)
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
}
