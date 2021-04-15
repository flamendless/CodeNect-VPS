#include "modules/node_to_code.hpp"

#include "ppk_assert.h"

namespace CodeNect::NodeToCode
{
std::string slot_to_str(NODE_SLOT& slot)
{
	switch (slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: return "bool"; break;
		case NODE_SLOT::INTEGER: return "int"; break;
		case NODE_SLOT::FLOAT: return "float"; break;
		case NODE_SLOT::DOUBLE: return "double"; break;
		case NODE_SLOT::STRING: return "const char*"; break;
	}
	PPK_ASSERT(false, "this should not be reached");
	return "";
}

std::string to_fixed_array(NodeArray* node_array)
{
	std::vector<std::string> vec;

	for (std::vector<NodeValue*>::iterator it = node_array->m_elements.begin();
		it != node_array->m_elements.end();
		it++)
	{
		NodeValue* val = *it;
		vec.push_back(val->get_value_str_ex());
	}

	std::string str = "{";
	for (int i = 0; i < vec.size(); i++)
	{
		str.append(vec[i]);

		if (i < vec.size() - 1)
			str.append(", ");
	}
	str.append("};");

	return str;
}

std::string to_dynamic_array(NodeArray* node_array, const std::string& insert_name)
{
	std::string str = "";

	for (int i = 0; i < vec.size(); i++)
	{
		str.append(vec[i]);

		if (i < vec.size() - 1)
			str.append(", ");
	}

	return str;
}

std::string comment(Node* node)
{
	const char* desc = node->m_desc;

	if (std::strlen(desc) != 0)
		return fmt::format("//{:s}", desc).append("\n");

	return "";
}

std::string node_var(NodeVariable* node_var)
{
	NodeValue* value = &node_var->m_value_orig;
	std::string type = value->get_type_str();
	const char* name = node_var->m_name;
	std::string val = value->get_value_str_ex();
	return fmt::format("{:s} {:s} = {:s}", type, name, val).append("\n");
}

std::string node_array(NodeArray* node_array)
{
	std::string str = "";
	std::string type = slot_to_str(node_array->m_slot);
	const char* name = node_array->m_name;

	switch (node_array->m_array)
	{
		case NODE_ARRAY::EMPTY: break;
		case NODE_ARRAY::FIXED:
		{
			std::string size = fmt::format("[{:d}]", node_array->m_fixed_size);
			std::string val = to_fixed_array(node_array);
			str = fmt::format("{:s} {:s}{:s} = {:s}", type, name, size, val).append("\n");
			break;
		}
		case NODE_ARRAY::DYNAMIC:
		{
			std::string size = fmt::format("[{:d}]", node_array->m_elements.size());
			std::string array_name = "";
			std::string init_name = "";
			std::string insert_name = "";

			switch (node_array->m_slot)
			{
				case NODE_SLOT::EMPTY: break;
				case NODE_SLOT::BOOL:
				{
					array_name = "DynamicArrayBool";
					init_name = "init_d_arr_bool";
					insert_name = "insert_bool";
					break;
				}
				case NODE_SLOT::INTEGER:
				{
					array_name = "DynamicArrayInt";
					init_name = "init_d_arr_int";
					insert_name = "insert_int";
					break;
				}
				case NODE_SLOT::FLOAT:
				{
					array_name = "DynamicArrayFloat";
					init_name = "init_d_arr_float";
					insert_name = "insert_float";
					break;
				}
				case NODE_SLOT::DOUBLE:
				{
					array_name = "DynamicArrayDouble";
					init_name = "init_d_arr_double";
					insert_name = "insert_double";
					break;
				}
				case NODE_SLOT::STRING:
				{
					array_name = "DynamicArrayString";
					init_name = "init_d_arr_string";
					insert_name = "insert_string";
					break;
				}
			}

			std::string val = to_dynamic_array(node_array, insert_name);
			std::string a = fmt::format("{:s} {:s};", array_name, name).append("\n");
			std::string b = fmt::format("{:s}(&{:s}, {:d});", init_name, name, size).append("\n");
			str = fmt::format("{:s}\n{:s}", a, b).append("\n");
			str.append(val).append("\n");
			break;
		}
	}

	return str;
}
}
