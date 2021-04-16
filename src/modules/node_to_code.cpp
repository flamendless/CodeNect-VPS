#include "modules/node_to_code.hpp"

#include "ppk_assert.h"
#include "modules/transpiler.hpp"

namespace CodeNect::NodeToCode
{
std::string indent(void)
{
	std::string str = "";

	for (int i = 0; i < Transpiler::level; i++)
		str.append("  ");

	return str;
}

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

std::string to_array(NodeArray* node_array)
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

std::string comment(Node* node)
{
	const char* desc = node->m_desc;

	if (std::strlen(desc) != 0)
		return fmt::format("{:s}//{:s}", indent(), desc).append("\n");

	return "";
}

std::string node_var(NodeVariable* node_var)
{
	NodeValue* value = &node_var->m_value_orig;
	std::string type = value->get_type_str();
	const char* name = node_var->m_name;
	std::string val = value->get_value_str_ex();
	return fmt::format("{:s}{:s} {:s} = {:s};", indent(), type, name, val).append("\n");
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
			std::string val = to_array(node_array);
			str = fmt::format("{:s}{:s} {:s}{:s} = {:s}", indent(), type, name, size, val).append("\n");
			break;
		}
		case NODE_ARRAY::DYNAMIC:
		{
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
					insert_name = "insert_bool_array";
					break;
				}
				case NODE_SLOT::INTEGER:
				{
					array_name = "DynamicArrayInt";
					init_name = "init_d_arr_int";
					insert_name = "insert_int_array";
					break;
				}
				case NODE_SLOT::FLOAT:
				{
					array_name = "DynamicArrayFloat";
					init_name = "init_d_arr_float";
					insert_name = "insert_float_array";
					break;
				}
				case NODE_SLOT::DOUBLE:
				{
					array_name = "DynamicArrayDouble";
					init_name = "init_d_arr_double";
					insert_name = "insert_double_array";
					break;
				}
				case NODE_SLOT::STRING:
				{
					array_name = "DynamicArrayString";
					init_name = "init_d_arr_string";
					insert_name = "insert_string_array";
					break;
				}
			}

			//{1, 2, 3};
			const int val_size = node_array->m_elements.size();
			std::string val = to_array(node_array);
			std::string val_name = std::string(name) + "_val";
			//DynamicArrayT name;
			std::string a = fmt::format("{:s} {:s};", array_name, name);
			//init_d_arr_T(&name, size)
			std::string b = fmt::format("{:s}(&{:s}, {:d});", init_name, name, val_size);
			//T val_name[] = {val}
			std::string c = fmt::format("{:s} {:s}[] = {:s};", type, val_name, val);
			//insert_T_array(&name, val, val_size);
			std::string d = fmt::format("{:s}(&{:s}, {:s}, {:d})", insert_name, name, val_name, val_size);

			str.append(indent()).append(a).append("\n")
				.append(indent()).append(b).append("\n")
				.append(indent()).append(c).append("\n")
				.append(indent()).append(d).append("\n");
			break;
		}
	}

	return str;
}
}
