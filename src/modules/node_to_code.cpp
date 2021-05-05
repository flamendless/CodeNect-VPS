#include "modules/node_to_code.hpp"

#include "ppk_assert.h"
#include "modules/transpiler.hpp"
#include "node/nodes.hpp"
#include "modules/debugger.hpp"

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
					std::pair<std::string, bool>t = Transpiler::get_temp_name(lhs_name.c_str(), true);
					std::string& buffer_name = t.first;
					if (t.second)
						return buffer_name;

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

std::string cmp_to_str(NODE_CMP& cmp)
{
	switch (cmp)
	{
		case NODE_CMP::EMPTY: break;
		case NODE_CMP::EQ: return "=="; break;
		case NODE_CMP::NEQ: return "!="; break;
		case NODE_CMP::GT: return ">"; break;
		case NODE_CMP::LT: return "<"; break;
		case NODE_CMP::GTE: return ">="; break;
		case NODE_CMP::LTE: return "<="; break;
		case NODE_CMP::OR: return "||"; break;
		case NODE_CMP::AND: return "&&"; break;
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

std::string ntc_var(NodeVariable* node_var)
{
	PLOGD << "Transpiling: " << node_var->m_name;
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
			NodeMath* out_node_math = dynamic_cast<NodeMath*>(out_node);
			NodeComparison* out_node_cmp = dynamic_cast<NodeComparison*>(out_node);
			NodeSize* out_node_size = dynamic_cast<NodeSize*>(out_node);
			NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);

			if (out_node_var)
				val = out_node_var->m_name;
			else if (out_node_prompt)
				val = fmt::format("{:s}.buffer", out_node_prompt->m_name);
			else if (out_node_print)
				val = fmt::format("\"{:s}\"", out_node_print->m_orig_str);
			else if (out_node_cast)
				val = NodeToCode::ntc_cast(out_node_cast, true, str);
			else if (out_node_op)
				val = NodeToCode::ntc_op(out_node_op, true, str);
			else if (out_node_math)
				val = NodeToCode::ntc_math(out_node_math, true, str);
			else if (out_node_cmp)
				val = NodeToCode::ntc_cmp(out_node_cmp, true, str);
			else if (out_node_size)
				val = NodeToCode::ntc_size(out_node_size, true, str);
			else if (out_node_arr_access)
				val = NodeToCode::ntc_array_access(out_node_arr_access, true, str);
		}
	}

	str.append(fmt::format("{:s}{:s} {:s} = {:s};", indent(), type, name, val).append("\n"));
	return str;
}

std::string ntc_size(NodeSize* node_size, bool val_only, std::string& pre)
{
	PLOGD << "Transpiling: " << node_size->m_name << ", val_only: " << val_only;
	std::string str = "";
	std::string rhs = "";

	//get the lhs
	for (const Connection& connection : node_size->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_size)
			continue;
		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeArray* out_node_arr = dynamic_cast<NodeArray*>(out_node);

		if (out_node_var)
		{
			if (out_node_var->m_value_orig.m_slot == +NODE_SLOT::STRING)
				rhs = fmt::format("strlen({:s})", out_node_var->m_name);
			else
			{
				std::string err = fmt::format("ERROR: node {:s} can't get size of variable that is not of type 'STRING'", node_size->m_name);
				Transpiler::add_message(std::move(err), OUTPUT_TYPE::ERROR, node_size, DOC_ID::VAR_SIZE);
				return "";
			}
		}
		else if (out_node_arr)
		{
			rhs = fmt::format("{:s}.used", out_node_arr->m_name);
		}
	}

	if (val_only)
		str = rhs;
	else
	{
		std::string d = fmt::format("int {:s} = {:s};", node_size->m_name, rhs);
		str.append(indent()).append(d).append("\n");
		Transpiler::m_declared.insert({node_size->m_name, true});
	}

	return str;
}

std::string ntc_cast(NodeCast* node_cast, bool val_only, std::string& pre)
{
	PLOGD << "Transpiling: " << node_cast->m_name << ", val_only: " << val_only;
	std::string str = "";
	NODE_SLOT in_slot = NODE_SLOT::_from_string(node_cast->m_in_slots[0].title);
	NODE_SLOT out_slot = NODE_SLOT::_from_string(node_cast->m_out_slots[0].title);
	std::string lhs_name = "";

	//get the input or value to be casted (lhs)
	for (const Connection& connection : node_cast->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_cast)
			continue;
		lhs_name = out_node->m_name;
		NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(out_node);
		NodeSize* node_size = dynamic_cast<NodeSize*>(out_node);
		// NodeArrayAccess* node_array_access = dynamic_cast<NodeArrayAccess*>(out_node);

		if (node_prompt)
			lhs_name.append(".buffer");
		else if (node_size)
		{
			if (Transpiler::m_declared.find(node_size->m_name) == Transpiler::m_declared.end())
			{
				//not found
				pre.append(indent())
					.append(NodeToCode::ntc_size(node_size, false, pre))
					.append("\n");
			}
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

std::string ntc_math(NodeMath* node_math, bool val_only, std::string& pre)
{
	PLOGD << "Transpiling: " << node_math->m_name << ", val_only: " << val_only;
	std::string str = "";
	std::string rhs = "";
	std::vector<std::string> v_refs;
	NODE_MATH& math = node_math->m_math;

	//possible LHS: node_var, node_op, node_arr_access, node_size
	for (const Connection& connection : node_math->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_math)
			continue;

		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);
		if (node_var)
			v_refs.push_back(node_var->m_name);
		else if (node_op)
		{
		}
	}

	bool is_single = math == +NODE_MATH::SIN ||
		math == +NODE_MATH::COS ||
		math == +NODE_MATH::TAN;

	if (is_single)
	{
		std::string& ref = v_refs.back();
		std::string fn = std::string(math._to_string());
		std::transform(fn.begin(), fn.end(), fn.begin(),
			[](unsigned char c){ return std::tolower(c); });
		rhs = fmt::format("{:s}({:s})", fn, ref);
	}
	else
	{
		if (v_refs.size() < 2)
		{
			std::string err = fmt::format("ERROR at node {:s}: using {:s} must have 2 inputs",
				node_math->m_name, math._to_string());
			Transpiler::add_message(std::move(err), OUTPUT_TYPE::ERROR, node_math, DOC_ID::MATH_REQ);
			return str;
		}

		std::string fn = "pow";
		std::string& ref1 = v_refs[v_refs.size() - 2];
		std::string& ref2 = v_refs[v_refs.size() - 1];

		if (math == +NODE_MATH::ROOT)
			ref2 = fmt::format("1.0/(int){:s}", ref2);

		rhs = fmt::format("{:s}({:s}, {:s})", fn, ref1, ref2);
	}

	if (val_only)
		str = rhs;

	return str;
}

std::string ntc_cmp(NodeComparison* node_cmp, bool val_only, std::string& pre)
{
	PLOGD << "Transpiling: " << node_cmp->m_name << ", val_only: " << val_only;
	std::string str = "";
	std::string rhs = "";
	std::string str_cmp = NodeToCode::cmp_to_str(node_cmp->m_cmp);
	std::vector<std::string> v_elements;

	//possible LHS: node_var, node_cast, node_math
	for (const Connection& connection : node_cmp->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_cmp)
			continue;

		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);

		if (out_node_var)
			v_elements.push_back(out_node_var->m_name);
	}

	if (v_elements.size() < 2)
	{
		std::string warning = fmt::format("NodeComparison {:s} must have at least 2 inputs",
				node_cmp->m_name);
		Transpiler::add_message(std::move(warning), OUTPUT_TYPE::WARNING, node_cmp, DOC_ID::CMP_REQ);
		return "";
	}

	for (int i = 0; i < v_elements.size(); i++)
	{
		std::string& str = v_elements[i];
		rhs.append(str);

		if (i < v_elements.size() - 1)
			rhs.append(fmt::format(" {:s} ", str_cmp));
	}

	if (val_only)
		str = fmt::format("({:s})", rhs);

	return str;
}

std::string ntc_op(NodeOperation* node_op, bool val_only, std::string& pre)
{
	PLOGD << "Transpiling: " << node_op->m_name << ", val_only: " << val_only;
	std::string str = "";
	std::string rhs = "";
	std::string op = node_op->get_op();
	NODE_SLOT type = NODE_SLOT::_from_string(node_op->m_in_slots[0].title);
	std::vector<std::string> v_elements;
	bool string_concat = false;

	//get LHS
	for (const Connection& connection : node_op->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_op)
			continue;

		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);
		NodeArrayAccess* out_node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);

		if (out_node_var)
			v_elements.push_back(out_node_var->m_name);
		else if (out_node_cast)
		{
			if (type == +NODE_SLOT::STRING)
			{
				std::string str_cast = NodeToCode::ntc_cast(out_node_cast, true, pre);
				v_elements.push_back(Transpiler::recent_temp);
				string_concat = true;
			}
			else
			{
				std::string str_cast = NodeToCode::ntc_cast(out_node_cast, true, pre);
				v_elements.push_back(str_cast);
			}
		}
		else if (out_node_arr_access)
		{
			if (type == +NODE_SLOT::STRING)
			{
			}
			else
			{
				std::string str_aa = NodeToCode::ntc_array_access(out_node_arr_access, true, pre);
				v_elements.push_back(str_aa);
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

std::string ntc_array_decls(NodeArray* node_array)
{
	PLOGD << "Transpiling: " << node_array->m_name;
	std::string str = "";
	const char* name = node_array->m_name;
	std::string pre_name = "";
	std::string type_name = "";
	std::string pre_method = "";

	if (node_array->m_array == +NODE_ARRAY::FIXED)
	{
		pre_name = "FixedArray";
		pre_method = "f";
	}
	else if (node_array->m_array == +NODE_ARRAY::DYNAMIC)
	{
		pre_name = "DynamicArray";
		pre_method = "d";
	}

	switch (node_array->m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: type_name = "Bool"; break;
		case NODE_SLOT::INTEGER: type_name = "Int"; break;
		case NODE_SLOT::FLOAT: type_name = "Float"; break;
		case NODE_SLOT::DOUBLE: type_name = "Double"; break;
		case NODE_SLOT::STRING: type_name = "String"; break;
	}
	std::string type_name_l = type_name;
	std::transform(type_name_l.begin(), type_name_l.end(), type_name_l.begin(),
			[](unsigned char c){ return std::tolower(c); });

	std::string array_name = pre_name + type_name;
	std::string init_name = pre_method + "_init_arr_" + type_name_l;

	//{1, 2, 3};
	const int val_size = node_array->m_elements.size();
	std::string val_name = std::string(name) + "_val";

	//Dynamic/FixedArrayT name;
	std::string a = fmt::format("{:s} {:s};", array_name, name);
	//init_d_arr_T(&name, size)
	std::string b = fmt::format("{:s}(&{:s}, {:d});", init_name, name, val_size);

	str.append(indent()).append(a).append("\n");
	str.append(indent()).append(b).append("\n");

	return str;
}

std::string ntc_array(NodeArray* node_array)
{
	PLOGD << "Transpiling: " << node_array->m_name;
	std::string str = "";
	std::string type = slot_to_str(node_array->m_slot);
	const char* name = node_array->m_name;
	std::string pre_name = "";
	std::string pre_method = "";
	std::string type_name = "";

	if (node_array->m_array == +NODE_ARRAY::FIXED)
	{
		pre_name = "FixedArray";
		pre_method = "f";
	}
	else if (node_array->m_array == +NODE_ARRAY::DYNAMIC)
	{
		pre_name = "DynamicArray";
		pre_method = "d";
	}

	switch (node_array->m_slot)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: type_name = "Bool"; break;
		case NODE_SLOT::INTEGER: type_name = "Int"; break;
		case NODE_SLOT::FLOAT: type_name = "Float"; break;
		case NODE_SLOT::DOUBLE: type_name = "Double"; break;
		case NODE_SLOT::STRING: type_name = "String"; break;
	}
	std::string type_name_l = type_name;
	std::transform(type_name_l.begin(), type_name_l.end(), type_name_l.begin(),
			[](unsigned char c){ return std::tolower(c); });

	std::string array_name = pre_name + type_name;
	std::string init_name = pre_method + "_init_arr_" + type_name_l;
	std::string insert_array = pre_method + "_insert_" + type_name_l + "_array";
	std::string insert_var = pre_method + "_insert_" + type_name_l;
	std::string free_method = pre_method + "_free_" + type_name_l;

	//{1, 2, 3};
	// const int val_size = node_array->m_elements.size();
	std::string val = NodeToCode::to_array(node_array);
	std::string val_name = std::string(name) + "_val";
	std::string size = fmt::format("(sizeof({:s})/sizeof({:s}[0]))", val_name, val_name);

	//Dynamic/FixedArrayT name;
	bool found_str = Transpiler::m_declared.find(node_array->m_name) != Transpiler::m_declared.end();
	if (!found_str)
	{
		std::string a = fmt::format("{:s} {:s};", array_name, name);
		str.append(indent()).append(a).append("\n");
	}

	//init_d_arr_T(&name, size)
	// std::string b = fmt::format("{:s}(&{:s}, {:d});", init_name, name, val_size);
	// str.append(indent()).append(b).append("\n");

	//T val_name[] = {val}
	bool found_arr = Transpiler::m_declared.find(val_name) != Transpiler::m_declared.end();
	if (!found_arr)
	{
		std::string c = fmt::format("{:s} {:s}[] = {:s};", type, val_name, val);
		str.append(indent()).append(c).append("\n");
		Transpiler::m_declared.insert({val_name, true});
	}

	//insert_T_array(&name, val, val_size);
	std::string d = fmt::format("{:s}(&{:s}, {:s}, {:s});", insert_array, name, val_name, size);
	str.append(indent()).append(d).append("\n");

	//get lhs value
	for (const Connection& connection : node_array->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_array)
			continue;
		NodeVariable* out_node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeArray* out_node_array = dynamic_cast<NodeArray*>(out_node);
		NodeCast* out_node_cast = dynamic_cast<NodeCast*>(out_node);

		if (out_node_var)
		{
			std::string ins = fmt::format("{:s}(&{:s}, {:s});",
					insert_var, name, out_node_var->m_name);
			str.append(indent()).append(ins).append("\n");
		}
		else if (out_node_array && out_node_array != node_array)
		{
			const int size_a = out_node_array->m_elements.size();
			const int size_b = out_node_array->m_other_elements.size();
			std::string str_size = std::to_string(size_a + size_b);
			std::string ins = fmt::format("{:s}(&{:s}, {:s}.array, {:s});",
					insert_array, name, out_node_array->m_name, str_size);
			str.append(indent()).append(ins).append("\n");
		}
		else if (out_node_cast)
		{
			std::string pre = "";
			std::string c = NodeToCode::ntc_cast(out_node_cast, true, pre);
			std::string ins = fmt::format("{:s}(&{:s}, {:s});",
					insert_var, name, c);
			if (pre.length() != 0)
				str.append(indent()).append(c).append("\n");
			str.append(indent()).append(ins).append("\n");
		}
	}

	Transpiler::m_declared.insert({node_array->m_name, true});
	Transpiler::m_array_init.insert({node_array->m_name, free_method});

	return str;
}

std::string ntc_array_access(NodeArrayAccess* node_array_access, bool val_only, std::string& pre)
{
	PLOGD << "Transpiling: " << node_array_access->m_name;
	std::string str = "";
	std::string index = std::to_string(node_array_access->m_index);
	std::string ref_name = "";
	NODE_SLOT out_slot = NODE_SLOT::_from_string(node_array_access->m_out_slots[0].title);
	std::string type = NodeToCode::slot_to_str(out_slot);

	//get the lhs
	for (const Connection& connection : node_array_access->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_array_access)
			continue;
		NodeArray* node_array = dynamic_cast<NodeArray*>(out_node);
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeCast* node_cast = dynamic_cast<NodeCast*>(out_node);
		NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);

		if (node_array)
		{
			ref_name = node_array->m_name;
			ref_name.append(".array");
		}
		else if (node_var)
			index = node_var->m_name;
		else if (node_cast)
			index = NodeToCode::ntc_cast(node_cast, true, pre);
		else if (node_math)
			index = NodeToCode::ntc_math(node_math, true, pre);
		else if (node_op)
			index = NodeToCode::ntc_op(node_op, true, pre);
	}

	std::string rhs = fmt::format("{:s}[{:s}]", ref_name, index);

	if (val_only)
		str = rhs;
	else
	{
		std::string d = fmt::format("{:s} {:s} = {:s};",
				type, node_array_access->m_name, rhs);
		str.append(indent()).append(d).append("\n");
		Transpiler::m_declared.insert({node_array_access->m_name, true});
	}

	return str;
}

std::string ntc_print(NodePrint* node_print)
{
	PLOGD << "Transpiling: " << node_print->m_name;
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
				NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);
				NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(out_node);
				NodeSize* node_size = dynamic_cast<NodeSize*>(out_node);
				NodeArrayAccess* node_array_access = dynamic_cast<NodeArrayAccess*>(out_node);

				if (node_var)
				{
					other_spec = node_var->m_value_orig.get_spec();
					other_val = node_var->m_name;
				}
				else if (node_prompt)
				{
					other_spec = "%s";
					other_val = fmt::format("{:s}.buffer", node_prompt->m_name);
				}
				else if (node_op)
				{
					NODE_SLOT slot = NODE_SLOT::_from_string(node_op->m_out_slots[0].title);
					other_spec = slot_to_spec(slot);
					std::string rhs = NodeToCode::ntc_op(node_op, true, str);
					other_val = fmt::format("({:s})", rhs);
				}
				else if (node_math)
				{
					NODE_SLOT slot = NODE_SLOT::_from_string(node_math->m_out_slots[0].title);
					other_spec = slot_to_spec(slot);
					std::string rhs = NodeToCode::ntc_math(node_math, true, str);
					other_val = fmt::format("({:s})", rhs);
				}
				else if (node_cmp)
				{
					other_spec = "%d"; //cmp out is always boolean
					std::string rhs = NodeToCode::ntc_cmp(node_cmp, true, str);
					other_val = fmt::format("({:s})", rhs);
				}
				else if (node_size)
				{
					other_spec = "%d"; //size is always int
					std::string rhs = NodeToCode::ntc_size(node_size, true, str);
					other_val = fmt::format("({:s})", rhs);
				}
				else if (node_array_access)
				{
					NODE_SLOT slot = NODE_SLOT::_from_string(node_array_access->m_out_slots[0].title);
					other_spec = slot_to_spec(slot);
					std::string rhs = NodeToCode::ntc_array_access(node_array_access, true, str);
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

std::string ntc_prompt(NodePrompt* node_prompt)
{
	PLOGD << "Transpiling: " << node_prompt->m_name;
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

std::string ntc_branch(NodeBranch* node_branch, bool is_in_else)
{
	PLOGD << "ntc_branch: " << node_branch->m_name;
	std::string str = "";
	std::string pre = "";
	std::string rhs = "";

	//get the lhs
	for (const Connection& connection : node_branch->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node_branch)
			continue;

		NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
		NodeComparison* node_cmp = dynamic_cast<NodeComparison*>(out_node);

		if (node_var)
			rhs = node_var->m_name;
		else if (node_cmp)
			rhs = NodeToCode::ntc_cmp(node_cmp, true, pre);
	}

	std::string str_statement = "";

	if (!is_in_else)
		str_statement = fmt::format("if ({:s})", rhs);
	else
		str_statement = "else";

	//opening brace and closing brace for code block is handled by Transpiler
	str.append(indent()).append(str_statement).append("\n");
		// .append(indent()).append("{").append("\n");
	// Transpiler::level++;

	// str.append(indent()).append("//code block").append("\n");
    //
	// Transpiler::level--;
	// str.append(indent()).append("}").append("\n");

	return str;
}
}
