#include "modules/node_val.hpp"

#include "imgui.h"
#include "plog/Log.h"

namespace CodeNect
{
void NodeValue::set(bool arg){ v_bool = arg; m_slot = NODE_SLOT::BOOL; }
void NodeValue::set(int arg){ v_int = arg; m_slot = NODE_SLOT::INTEGER; }
void NodeValue::set(float arg){ v_float = arg; m_slot = NODE_SLOT::FLOAT; }
void NodeValue::set(double arg){ v_double = arg; m_slot = NODE_SLOT::DOUBLE; }
void NodeValue::set(const char* arg){ v_string = arg; m_slot = NODE_SLOT::STRING; }

void NodeValue::copy(NODE_SLOT& slot)
{
	switch (slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: set(true); break;
		case NODE_SLOT::INTEGER: set(0); break;
		case NODE_SLOT::FLOAT: set((float)0); break;
		case NODE_SLOT::DOUBLE: set((double)0); break;
		case NODE_SLOT::STRING: set(""); break;
	}
}

void NodeValue::to_bool(const char* str)
{
	if (std::strcmp(str, "true") == 0)
		set(true);
	else if (std::strcmp(str, "false") == 0)
		set(false);
	else
		PLOGE << "unable to convert '" << str << "' to bool";
}

void NodeValue::to_int(const char* str)
{
	int i = std::stoi(str);
	set(i);
}

void NodeValue::to_float(const char* str)
{
	float i = std::stof(str);
	set(i);
}

void NodeValue::to_double(const char* str)
{
	double i = std::stod(str);
	set(i);
}

void* NodeValue::get_value()
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
