#include "modules/node_to_code.hpp"

#include "ppk_assert.h"
#include "modules/transpiler.hpp"
#include "node/nodes.hpp"

namespace CodeNect::NodeToCode
{
using fn = std::function<std::string(std::string&, std::string&)>;
using m = std::map<std::string, fn>;

//in -> out
std::map<std::string, m> m_cast
{
	{"BOOL", {
			{"INTEGER", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("({:s} ? 1 : 0)", lhs_name); },
			},
			{"STRING", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("bool_to_string({:s})", lhs_name); },
			}
		}
	},
	{"INTEGER", {
			{"BOOL", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("int_to_bool({:s})", lhs_name); },
			},
			{"FLOAT", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("(float)({:s})", lhs_name); },
			},
			{"DOUBLE", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("(double)({:s})", lhs_name); },
			},
			{"STRING", [](std::string& lhs_name, std::string& pre) -> std::string
				{
					std::string buffer_name = Transpiler::get_temp_name(lhs_name.c_str());
					pre.append(indent())
						.append(fmt::format("char {:s}[{:d}];", buffer_name, 256)).append("\n")
						.append(indent())
						.append(fmt::format("sprintf({:s}, \"%d\", {});", buffer_name, lhs_name)).append("\n");
					return fmt::format("{:s}", buffer_name);
				},
			}
		},
	},

	{"FLOAT", {
			{"INTEGER", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("(int){:s}", lhs_name); },
			},
			{"DOUBLE", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("(double){:s}", lhs_name); },
			},
			{"STRING", [](std::string& lhs_name, std::string& pre) -> std::string
				{
					std::string buffer_name = Transpiler::get_temp_name(lhs_name.c_str());
					pre.append(indent())
						.append(fmt::format("char {:s}[{:d}];", buffer_name, 256)).append("\n")
						.append(indent())
						.append(fmt::format("sprintf({:s}, \"%f\", {});", buffer_name, lhs_name)).append("\n");
					return fmt::format("{:s}", buffer_name);
				},
			}
		}
	},

	{"DOUBLE", {
			{"INTEGER", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("(int){:s}", lhs_name); },
			},
			{"FLOAT", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("(float){:s}", lhs_name); },
			},
			{"STRING", [](std::string& lhs_name, std::string& pre) -> std::string
				{
					std::string buffer_name = Transpiler::get_temp_name(lhs_name.c_str());
					pre.append(indent())
						.append(fmt::format("char {:s}[{:d}];", buffer_name, 256)).append("\n")
						.append(indent())
						.append(fmt::format("sprintf({:s}, \"%lf\", {});", buffer_name, lhs_name)).append("\n");
					return fmt::format("{:s}", buffer_name);
				},
			}
		}
	},

	{"STRING", {
			{"BOOL", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("string_to_bool({:s})", lhs_name); },
			},
			{"INTEGER", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("strtol({:s}, NULL, 10)", lhs_name); },
			},
			{"FLOAT", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("strtof({:s}, NULL)", lhs_name); },
			},
			{"DOUBLE", [](std::string& lhs_name, std::string&) -> std::string
				{ return fmt::format("strtod({:s}, NULL)", lhs_name); },
			},
		}
	},
};

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

std::string slot_to_spec(NODE_SLOT& slot)
{
	switch (slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: return "%d"; break;
		case NODE_SLOT::INTEGER: return "%d"; break;
		case NODE_SLOT::FLOAT: return "%f"; break;
		case NODE_SLOT::DOUBLE: return "%lf"; break;
		case NODE_SLOT::STRING: return "%s"; break;
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
	std::string str = "";
	NodeValue* value = &node_var->m_value;
	std::string type = value->get_type_str();
	const char* name = node_var->m_name;
	std::string val = value->get_value_str_ex();

	//get the variable from the connection (lhs)
	for (const Connection& connection : node_var->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node != node_var)
		{
			NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
			NodePrompt* out_node_prompt = dynamic_cast<NodePrompt*>(out_node);
			NodePrint* out_node_print = dynamic_cast<NodePrint*>(out_node);
			NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);
			NodeOperation* out_node_op = dynamic_cast<NodeOperation*>(out_node);

			if (out_node_var)
				val = out_node_var->m_name;
			else if (out_node_prompt)
				val = fmt::format("{:s}.buffer", out_node_prompt->m_name);
			else if (out_node_print)
				val = fmt::format("\"{:s}\"", out_node_print->m_orig_str);
			else if (out_node_cast)
				val = NodeToCode::node_cast(out_node_cast, true, str);
			else if (out_node_op)
				val = NodeToCode::node_op(out_node_op, true, str);
		}
	}

	str.append(fmt::format("{:s}{:s} {:s} = {:s};", indent(), type, name, val).append("\n"));
	return str;
}

std::string node_cast(NodeCast* node_cast, bool val_only, std::string& pre)
{
	std::string str = "";
	NODE_SLOT in_slot = NODE_SLOT::_from_string(node_cast->m_in_slots[0].title);
	NODE_SLOT out_slot = NODE_SLOT::_from_string(node_cast->m_out_slots[0].title);
	std::string lhs_name = "";

	//get the input or value to be casted (lhs)
	for (const Connection& connection : node_cast->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node != node_cast)
		{
			lhs_name = out_node->m_name;
			NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(out_node);
			if (node_prompt)
				lhs_name.append(".buffer");
		}
	}

	std::string type = NodeToCode::slot_to_str(out_slot);
	std::string rhs = m_cast[in_slot._to_string()][out_slot._to_string()](lhs_name, pre);

	if (val_only)
		str.append(fmt::format("{:s}", rhs));
	else
	{
		str.append(indent())
			.append(fmt::format("{:s} {:s} = {:s}", type, node_cast->m_name, rhs))
			.append("\n");
	}

	return str;
}

std::string node_op(NodeOperation* node_op, bool val_only, std::string& pre)
{
	std::string str = "";
	std::string rhs = "";
	std::string op = node_op->get_op();
	NODE_SLOT type = NODE_SLOT::_from_string(node_op->m_in_slots[0].title);
	std::vector<std::string> v_elements;
	bool string_concat = false;

	//possible LHS: node_var, node_cast
	for (const Connection& connection : node_op->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_op)
			continue;

		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);

		if (out_node_var)
			v_elements.push_back(out_node_var->m_name);

		if (out_node_cast)
		{
			if (type == +NODE_SLOT::STRING)
			{
				std::string str_cast = NodeToCode::node_cast(out_node_cast, true, pre);
				v_elements.push_back(Transpiler::recent_temp);
				string_concat = true;
				//get all the references (lhs)
				// for (const Connection& connection : out_node_cast->m_connections)
				// {
				// 	Node* in_node = static_cast<Node*>(connection.in_node);
				// 	if (in_node != out_node_cast)
				// 		v_elements.push_back(in_node->m_name);
				// }

  	  	  	  // char* buffer3 = malloc(sizeof(char) * (strlen(buffer) + strlen(buffer2)));
  	  	  	  // strcpy(buffer3, buffer);
  	  	  	  // strcat(buffer3, buffer2);
              //
			  // std::string size_a = fmt::format("sizeof({:s})", a);
			  // std::string size_b = fmt::format("sizeof({:s})", b);
			}
			else
			{
				std::string str_cast = NodeToCode::node_cast(out_node_cast, true, pre);
				v_elements.push_back(str_cast);
			}
		}
	}

	if (string_concat)
	{
		std::string buf_name = Transpiler::get_temp_name(node_op->m_name);
		std::string str_size = "";
		std::string str_cat = "";
		str_cat.append(fmt::format("strcpy({:s}, {:s});", buf_name, v_elements[0]))
			.append("\n");

		for (int i = 0; i < v_elements.size(); i++)
		{
			std::string& ref = v_elements[i];
			str_size.append(fmt::format("strlen({:s})", ref));

			if (i != 0)
			{
				str_cat.append(indent())
					.append(fmt::format("strcat({:s}, {:s});", buf_name, ref))
					.append("\n");
			}

			if (i < v_elements.size() - 1)
				str_size.append(" + ");
		}
		std::string str_buf = fmt::format("char* {:s} = malloc(sizeof(char) * ({:s}));", buf_name, str_size);
		pre.append(indent()).append(str_buf).append("\n");
		pre.append(indent()).append(str_cat).append("\n");
		rhs = buf_name;
	}
	else
	{
		for (int i = 0; i < v_elements.size(); i++)
		{
			std::string& str = v_elements[i];
			rhs.append(str);

			if (i < v_elements.size() - 1)
				rhs.append(" ").append(op).append(" ");
		}
	}

	if (val_only)
		str = rhs;

	return str;
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
				NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);

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

				if (node_op)
				{
					NODE_SLOT slot = NODE_SLOT::_from_string(node_op->m_out_slots[0].title);
					other_spec = slot_to_spec(slot);
					std::string rhs = NodeToCode::node_op(node_op, true, str);
					other_val = fmt::format("({:s})", rhs);
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
