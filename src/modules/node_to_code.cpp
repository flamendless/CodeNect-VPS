#include "modules/node_to_code.hpp"

#include "ppk_assert.h"
#include "modules/transpiler.hpp"
#include "node/nodes.hpp"

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
	str.append("}");

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
	NodeValue* value = &node_var->m_value;
	std::string type = value->get_type_str();
	const char* name = node_var->m_name;
	std::string val = value->get_value_str_ex();

	//get the variable from the connection
	for (const Connection& connection : node_var->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node != node_var)
		{
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			if (out_node_var)
				val = out_node_var->m_name;
		}
	}

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
			str = fmt::format("{:s}{:s} {:s}{:s} = {:s};", indent(), type, name, size, val).append("\n");
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
			std::string d = fmt::format("{:s}(&{:s}, {:s}, {:d});", insert_name, name, val_name, val_size);

			str.append(indent()).append(a).append("\n")
				.append(indent()).append(b).append("\n")
				.append(indent()).append(c).append("\n")
				.append(indent()).append(d).append("\n");
			break;
		}
	}

	return str;
}

std::string node_print(NodePrint* node_print)
{
	std::string str = "";
	std::string spec = "%s";
	std::string value = fmt::format("\"{:s}\"", node_print->m_orig_str);
	std::string newline = "";

	if (node_print->m_override || node_print->m_append)
	{
		std::string other_val = "";
		std::string other_spec = "";
		for (const Connection& connection : node_print->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			if (out_node != node_print)
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
				NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(out_node);

				if (node_var)
				{
					other_spec = node_var->m_value_orig.get_spec();
					other_val = node_var->m_name;
				}

				if (node_prompt)
				{
					other_spec = "%s";
					other_val = fmt::format("{:s}.buffer", node_prompt->m_name);
				}
			}
		}

		if (node_print->m_override)
		{
			if (other_spec.length() != 0)
				spec = other_spec;
			if (other_val.length() != 0)
				value = other_val;
		}
		else if (node_print->m_append)
		{
			spec = "%s";
			spec.append(other_spec);
			if (other_val.length() != 0)
				value = fmt::format("\"{}\", {}", node_print->m_orig_str, other_val);
		}
	}

	if (node_print->m_append_newline)
		newline = "\\n";

	std::string print = fmt::format("printf(\"{:s}{:s}\", {:s});", spec, newline, value);
	str.append(indent()).append(print).append("\n");

	return str;
}

std::string node_prompt(NodePrompt* node_prompt)
{
	const char* name = node_prompt->m_name;
	std::string str = "";
	std::string value = "";
	std::string is_newline = "false";
	std::string pre = "";
	int size = 128;

	if (!node_prompt->m_override)
	{
		value = fmt::format("\"{:s}\"", node_prompt->m_orig_str);
	}
	else
	{
		//get the reference or the "from"
		for (const Connection& connection : node_prompt->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			if (out_node != node_prompt)
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
				if (node_var)
				{
					NodeValue* val = &node_var->m_value;
					switch (val->m_slot)
					{
						case NODE_SLOT::EMPTY: break;
						case NODE_SLOT::BOOL:
						{
							value = fmt::format("bool_to_string({:s})", node_var->m_name);
							break;
						}
						case NODE_SLOT::INTEGER:
						case NODE_SLOT::FLOAT:
						case NODE_SLOT::DOUBLE:
						{
							std::string out_name = Transpiler::get_temp_name(name);
							std::string spec = val->get_spec();
							pre.append(fmt::format("{:s}char {:s}[32];",
								indent(), out_name)).append("\n");
							pre.append(fmt::format("{:s}sprintf({:s}, \"{:s}\", {:s});",
								indent(), out_name, spec, node_var->m_name));
							pre.append("\n");
							value = out_name;
							break;
						}
						case NODE_SLOT::STRING:
						{
							value = node_var->m_name;
							break;
						}
					}
				}
			}
		}
	}

	if (node_prompt->m_append_newline)
		is_newline = "true";

	std::string decl = fmt::format("Prompt {:s};", name);
	std::string init = fmt::format("init_prompt(&{:s}, {:d});", name, size);
	std::string run = fmt::format("run_prompt(&{:s}, {:s}, {:s});", name, value, is_newline);

	str.append(indent()).append(decl).append("\n")
		.append(indent()).append(init).append("\n")
		.append(pre)
		.append(indent()).append(run).append("\n");

	return str;
}
}
