#include "modules/transpiler.hpp"
#include "modules/node_to_code.hpp"

#include <stdio.h>
#include <functional>
#include "plog/Log.h"
#include "ppk_assert.h"
#include "IconsFontAwesome5.h"
#include "fmt/format.h"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "node/nodes.hpp"
#include "ui/terminal.hpp"
#include "ui/alert.hpp"
#include "modules/filesystem.hpp"
#include "modules/templates.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_math.hpp"
#include "node/node_array.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"
#include "node/node_array_access.hpp"
#include "core/project.hpp"
#include "core/utils.hpp"
#include "modules/debugger.hpp"

namespace CodeNect
{
TCCState* Transpiler::tcc_state = nullptr;
std::string Transpiler::output_code = "";
std::string Transpiler::runnable_code = "";
std::vector<std::pair<std::string, OUTPUT_TYPE>> Transpiler::v_output;
std::vector<std::string> Transpiler::v_declarations;
std::map<std::string, bool> Transpiler::m_temp_names;
std::map<std::string, bool> Transpiler::m_declared;
std::map<std::string, std::string> Transpiler::m_array_init;
int Transpiler::level = 0;
bool Transpiler::has_ran = false;
bool Transpiler::has_compiled = false;
std::string Transpiler::recent_temp = "";

int Transpiler::init(void)
{
	Transpiler::tcc_state = tcc_new();

	if (!Transpiler::tcc_state)
	{
		PLOGE << "Could not create TCC state";
		return RES_FAIL;
	}

	tcc_set_output_type(Transpiler::tcc_state, TCC_OUTPUT_EXE);

	return RES_SUCCESS;
}

void Transpiler::register_commands(void)
{
	Command* cmd_compile = new Command("Compile", "compile the current code", ICON_FA_HAMMER);
	cmd_compile->set_fn(Transpiler::compile);
	cmd_compile->m_close_command_palette = true;

	Command* cmd_run = new Command("Run", "run the current code", ICON_FA_PLAY);
	cmd_run->set_fn(Transpiler::run);
	cmd_run->m_close_command_palette = true;

	Commands::register_cmd(*cmd_compile);
	Commands::register_cmd(*cmd_run);
}

void Transpiler::error(const char* str)
{
	PLOGE << str;
	Transpiler::v_output.push_back({str, OUTPUT_TYPE::ERROR});
}

void Transpiler::warning(const char* str, Node* node)
{
	PLOGW << str;
	Transpiler::v_output.push_back({str, OUTPUT_TYPE::WARNING});
	const int index = Transpiler::v_output.size();
	Debugger::m_nodes.insert({index, node});
}

std::string Transpiler::get_temp_name(const char* name)
{
	std::string out_name = fmt::format("temp_str_{}", name);
	int i = 0;
	while (Transpiler::m_temp_names.find(out_name) !=
		Transpiler::m_temp_names.end())
	{
		//found
		out_name.append("_" + std::to_string(i));
		i++;
	}
	Transpiler::m_temp_names.insert({out_name, true});
	Transpiler::recent_temp = out_name;

	return out_name;
}

bool Transpiler::is_valid_decls(Node* node)
{
	bool valid = false;
	switch (node->m_kind)
	{
		case NODE_KIND::VARIABLE: valid = true; break;
		// case NODE_KIND::DS: valid = true; break;
		case NODE_KIND::ACTION: valid = true; break;
	}

	return valid;
}

std::pair<std::string, bool> Transpiler::get_temp_name(const char* name, bool reuse)
{
	std::string out_name = fmt::format("temp_str_{}", name);
	bool is_found = false;
	int i = 0;
	while (Transpiler::m_temp_names.find(out_name) !=
		Transpiler::m_temp_names.end())
	{
		//found
		is_found = true;
		if (!reuse)
			out_name.append("_" + std::to_string(i));
		else
			break;
	}
	Transpiler::m_temp_names.insert({out_name, true});
	Transpiler::recent_temp = out_name;

	return std::make_pair<>(out_name, is_found);
}

//fill the string for includes and structs
void Transpiler::set_pre_entry(std::string& str_incl, std::string& str_structs, bool is_tcc)
{
	//includes
	bool has_io = false;
	bool has_math = false;
	bool has_cast = false;
	bool has_stdlib = false;
	bool has_d_a_bool = false;
	bool has_d_a_int = false;
	bool has_d_a_float = false;
	bool has_d_a_double = false;
	bool has_d_a_str = false;
	bool has_f_a_bool = false;
	bool has_f_a_int = false;
	bool has_f_a_float = false;
	bool has_f_a_double = false;
	bool has_f_a_str = false;
	bool has_prompt = false;

	if (is_tcc)
		str_incl.append(Templates::incl_tcc).append("\n");

	str_incl.append(Templates::incl_bool);

	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodePrint* node_print = dynamic_cast<NodePrint*>(*it);
		NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(*it);
		NodeMath* node_math = dynamic_cast<NodeMath*>(*it);
		NodeArray* node_array = dynamic_cast<NodeArray*>(*it);
		NodeCast* node_cast = dynamic_cast<NodeCast*>(*it);

		if (!has_io && (node_print || node_prompt) && !is_tcc)
		{
			str_incl.append(Templates::incl_stdio);
			has_io = true;
		}

		if (!has_cast && node_cast)
		{
			str_incl.append(Templates::incl_string);
			str_structs.append(Templates::def_cast);
			has_cast = true;
		}

		if (!has_stdlib && node_cast)
		{
			str_incl.append(Templates::incl_stdlib);
			has_stdlib = true;
		}

		if (!has_prompt && node_prompt)
		{
			str_structs.append(Templates::struct_prompt);
			has_prompt = true;
			continue;
		}

		if (!has_math && node_math)
		{
			str_incl.append(Templates::incl_math);
			has_math = true;
			continue;
		}

		if (node_array && node_array->m_array == +NODE_ARRAY::FIXED)
		{
			if (!has_stdlib && !is_tcc)
			{
				str_incl.append(Templates::incl_stdlib);
				has_stdlib = true;
			}

			switch (node_array->m_slot)
			{
				case NODE_SLOT::EMPTY: break;
				case NODE_SLOT::BOOL:
				{
					if (!has_f_a_bool)
					{
						str_structs.append(Templates::f_arr_bool);
						has_f_a_bool = true;
					}
					break;
				}
				case NODE_SLOT::INTEGER:
				{
					if (!has_f_a_int)
					{
						str_structs.append(Templates::f_arr_int);
						has_f_a_int = true;
					}
					break;
				}
				case NODE_SLOT::FLOAT:
				{
					if (!has_f_a_float)
					{
						str_structs.append(Templates::f_arr_float);
						has_f_a_float = true;
					}
					break;
				}
				case NODE_SLOT::DOUBLE:
				{
					if (!has_f_a_double)
					{
						str_structs.append(Templates::f_arr_double);
						has_f_a_double = true;
					}
					break;
				}
				case NODE_SLOT::STRING:
				{
					if (!has_f_a_str)
					{
						str_structs.append(Templates::f_arr_string);
						has_f_a_str = true;
					}
					break;
				}
			}
		}

		if (node_array && node_array->m_array == +NODE_ARRAY::DYNAMIC)
		{
			if (!has_stdlib && !is_tcc)
			{
				str_incl.append(Templates::incl_stdlib);
				has_stdlib = true;
			}

			switch (node_array->m_slot)
			{
				case NODE_SLOT::EMPTY: break;
				case NODE_SLOT::BOOL:
				{
					if (!has_d_a_bool)
					{
						str_structs.append(Templates::d_arr_bool);
						has_d_a_bool = true;
					}
					break;
				}
				case NODE_SLOT::INTEGER:
				{
					if (!has_d_a_int)
					{
						str_structs.append(Templates::d_arr_int);
						has_d_a_int = true;
					}
					break;
				}
				case NODE_SLOT::FLOAT:
				{
					if (!has_d_a_float)
					{
						str_structs.append(Templates::d_arr_float);
						has_d_a_float = true;
					}
					break;
				}
				case NODE_SLOT::DOUBLE:
				{
					if (!has_d_a_double)
					{
						str_structs.append(Templates::d_arr_double);
						has_d_a_double = true;
					}
					break;
				}
				case NODE_SLOT::STRING:
				{
					if (!has_d_a_str)
					{
						str_structs.append(Templates::d_arr_string);
						has_d_a_str = true;
					}
					break;
				}
			}
		}
	}
}

void Transpiler::transpile_decls(std::vector<Node*>& v, std::string& output)
{
	//the following are only possible to be nodes without any LHS
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;
		Transpiler::m_declared.insert({node->m_name, true});

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(node);
				output.append(NodeToCode::comment(node));
				output.append(NodeToCode::ntc_var(node_var));
				break;
			}

			case NODE_KIND::DS:
			{
				NodeDS* node_ds = static_cast<NodeDS*>(node);
				switch (node_ds->m_ds)
				{
					case NODE_DS::EMPTY: break;
					case NODE_DS::ARRAY:
					{
						NodeArray* node_array = dynamic_cast<NodeArray*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_array(node_array));
						break;
					}
				}
				break;
			}

			case NODE_KIND::ACTION:
			{
				NodeAction* node_action = static_cast<NodeAction*>(node);
				switch (node_action->m_action)
				{
					case NODE_ACTION::EMPTY: break;
					case NODE_ACTION::PRINT:
					{
						NodePrint* node_print = dynamic_cast<NodePrint*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_print(node_print));
						break;
					}
					case NODE_ACTION::PROMPT:
					{
						NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_prompt(node_prompt));
						break;
					}
				}
				break;
			}

			default:
			{
				std::string warning = fmt::format("WARNING: node {:s} is parsed as part of decls but it is not transpiled", node->m_name);
				Transpiler::warning(warning.c_str(), node);
				break;
			}
		}
	}
}

void Transpiler::transpile_decls_array(std::vector<Node*>& v, std::string& output)
{
	//the following are only possible to be nodes without any LHS
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;
		NodeArray* node_array = static_cast<NodeArray*>(node);
		bool found = Transpiler::m_declared.find(node_array->m_name) != Transpiler::m_declared.end();
		if (!found)
		{
			output.append(NodeToCode::comment(node));
			output.append(NodeToCode::ntc_array_decls(node_array));
			Transpiler::m_declared.insert({node_array->m_name, true});
		}
	}
}

void Transpiler::transpile(std::vector<Node*>& v, std::string& output)
{
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;

		if (Transpiler::m_declared.find(node->m_name) != Transpiler::m_declared.end())
		{
			//found
			NodeArray* node_array = dynamic_cast<NodeArray*>(node);
			if (!node_array)
			{
				PLOGW << node->m_name << " is declared already. Skipping.";
				continue;
			}
		}
		else
			Transpiler::m_declared.insert({node->m_name, true});

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = static_cast<NodeVariable*>(node);
				output.append(NodeToCode::comment(node));
				output.append(NodeToCode::ntc_var(node_var));
				output.append("\n");
				break;
			}

			case NODE_KIND::DS:
			{
				NodeDS* node_ds = static_cast<NodeDS*>(node);
				switch (node_ds->m_ds)
				{
					case NODE_DS::EMPTY: break;
					case NODE_DS::ARRAY:
					{
						NodeArray* node_array = static_cast<NodeArray*>(node);
						bool found = Transpiler::m_array_init.find(node_array->m_name) != Transpiler::m_array_init.end();
						if (found)
							continue;
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_array(node_array));
						output.append("\n");
						break;
					}
				}
				break;
			}

			case NODE_KIND::ACTION:
			{
				NodeAction* node_action = static_cast<NodeAction*>(node);
				switch (node_action->m_action)
				{
					case NODE_ACTION::EMPTY: break;
					case NODE_ACTION::PRINT:
					{
						NodePrint* node_print = static_cast<NodePrint*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_print(node_print));
						output.append("\n");
						break;
					}
					case NODE_ACTION::PROMPT:
					{
						NodePrompt* node_prompt = static_cast<NodePrompt*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_prompt(node_prompt));
						output.append("\n");
						break;
					}
				}
				break;
			}

			case NODE_KIND::CAST: break;
			case NODE_KIND::OPERATION: break;
			case NODE_KIND::COMPARISON: break;
			case NODE_KIND::GET:
			{
				NodeGet* node_get = static_cast<NodeGet*>(node);
				switch (node_get->m_get)
				{
					case NODE_GET::EMPTY: break;
					case NODE_GET::SIZE:
					{
						NodeSize* node_size = static_cast<NodeSize*>(node_get);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_size(node_size, false, output));
						output.append("\n");
						break;
					}
					case NODE_GET::ARRAY_ACCESS:
					{
						NodeArrayAccess* node_array_access = static_cast<NodeArrayAccess*>(node_get);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_array_access(node_array_access, false, output));
						output.append("\n");
						break;
					}
				}
				break;
			}
		}
	}
}

//TODO maybe arrange sequence by length of sequence??
//for example:
//sequence A : 5 nodes
//sequence B : 3 nodes
//Sequence B should be first before Sequence A
std::vector<std::vector<Node*>> Transpiler::get_v_sequence(std::vector<Node*>& v_start)
{
	//get the independent sequence/chain of nodes
	std::vector<std::vector<Node*>> v_out;
	for (std::vector<Node*>::iterator it = v_start.begin();
		it != v_start.end();
		it++)
	{
		Node* node = *it;
		std::vector<Node*> v_seq = Transpiler::get_sequence(node);

		if (v_seq.size() == 0)
		{
			v_seq.push_back(node);
			PLOGW << "v_seq size is 0 so " << node->m_name << " was added to sequence";
		}
		v_out.push_back(v_seq);
	}

	return v_out;
}

std::vector<Node*> Transpiler::get_sequence(Node* start_node)
{
	std::vector<Node*> v;
	PLOGW << "seq. start_node: " << start_node->m_name;
	for (const Connection& connection : start_node->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		if (in_node != start_node)
		{
			unsigned int count = Nodes::count_node_dep(in_node);
			PLOGD << "\t" << in_node->m_name << " = " << count;
			if (count == 1)
			{
				v.push_back(in_node);
				std::vector<Node*> v2 = Transpiler::get_sequence(in_node);
				v.insert(v.end(), v2.begin(), v2.end());
			}
		}
	}
	PLOGW << "seq. end";
	return v;
}

std::vector<Node*> Transpiler::get_rest(std::vector<std::vector<Node*>>& v_start)
{
	//get the rest to be transpiled using the last node in the sequence/chain
	std::vector<Node*> v_out;
	PLOGW << "begin rest. vv_size = " << v_start.size();
	for (std::vector<Node*>& v : v_start)
	{
		PLOGD << "v_size = " << v.size();
		for (Node* &node : v)
		{
			PLOGD << node->m_name;
			for (const Connection& connection : node->m_connections)
			{
				Node* in_node = static_cast<Node*>(connection.in_node);
				if (in_node != node)
				{
					PLOGD << "\t" << "in_node: " << in_node->m_name;
					//check if node was already added
					if (std::find(v_out.begin(), v_out.end(), in_node) != v_out.end())
					{
						PLOGW << "\t" << in_node->m_name << " was already added to rest. Skipping";
						continue;
					}
					v_out.push_back(in_node);
					PLOGW << "\t" << in_node->m_name << " is added to rest";
				}
			}
		}
	}
	PLOGW << "end rest";
	return v_out;
}

void Transpiler::arrange_v(std::vector<Node*>& v)
{
	std::sort(v.begin(), v.end(),
		[](Node* &a, Node* &b) -> bool
		{
			NodeArray* node_array_a = dynamic_cast<NodeArray*>(a);
			if (!node_array_a)
				return false;

			bool has_array_dep = false;
			for (const Connection& connection : a->m_connections)
			{
				Node* out_node = static_cast<Node*>(connection.out_node);
				if (out_node == a)
					continue;
				NodeArray* node_array = dynamic_cast<NodeArray*>(out_node);
				if (node_array && node_array == b)
				{
					PLOGD << "swapped " << a->m_name << " with " << b->m_name;
					has_array_dep = true;
					break;
				}
			}
			return !has_array_dep;
		});
}

void Transpiler::build_runnable_code(std::string& out, bool is_tcc)
{
	std::string str_incl = "";
	std::string str_structs = "";
	std::string str_entry = "";
	std::string str_decls = "";
	std::string str_next = "";
	std::string str_closing = "";
	std::string str_free = "";

	Transpiler::m_temp_names.clear();
	Transpiler::set_pre_entry(str_incl, str_structs, is_tcc);

	//entry point
	str_entry.append("int main()").append("\n");
	str_entry.append("{").append("\n");

	//store
	std::vector<Node*> v_decls;
	std::vector<Node*> v_decls_array;

	//find all nodes that do NOT have any LHS, this means that they are for declarations
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = *it;
		if (Nodes::check_if_no_lhs(node))
		{
			if (!Transpiler::is_valid_decls(node))
			{
				std::string warning = fmt::format("Node {:s} is not a valid declaration because it needs an inputs",
						node->m_name);
				Transpiler::warning(warning.c_str(), node);
			}
			else
				v_decls.push_back(node);
		}

		//check if inputs are satisfied
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(node);
		if (node_op)
		{
			const int count = Nodes::count_node_dep(node_op);
			if (count < 2)
			{
				std::string warning = fmt::format("Node {:s} must have two inputs. Got {:d}",
						node->m_name, count);
				Transpiler::warning(warning.c_str(), node_op);
			}
		}

		//exception: still define all arrays
		NodeArray* node_array = dynamic_cast<NodeArray*>(node);
		if (node_array)
			v_decls_array.push_back(node);
	}

	//begin transpiling
	Transpiler::level++;
#if 0
		PLOGD << "---start decls---";
		for (Node* &node : v_decls)
			PLOGD << node->m_name;
		PLOGD << "---end decls---";
#endif
	PLOGD << "Transpiling decls...";
	Transpiler::transpile_decls(v_decls, str_decls);
	PLOGD << "Transpiled decls";

#if 0
		PLOGD << "---start decls array---";
		for (Node* &node : v_decls_array)
			PLOGD << node->m_name;
		PLOGD << "---end decls array---";
#endif
	PLOGD << "Transpiling decls array...";
	Transpiler::transpile_decls_array(v_decls_array, str_decls);
	PLOGD << "Transpiled decls array";

	//repeat
	int pass = 1;
	std::vector<Node*> v_start = v_decls;
	while (1)
	{
		PLOGD << "PASS #" << pass;
		std::vector<std::vector<Node*>> v_tmp_seq = Transpiler::get_v_sequence(v_start);
		for (std::vector<Node*>& v : v_tmp_seq)
			Transpiler::arrange_v(v);

#if 0
		PLOGD << "Sequence. size = " << v_tmp_seq.size();
		for (std::vector<Node*>& v : v_tmp_seq)
		{
			PLOGD << "--start seq--";
			for (Node* &node : v)
				PLOGD << "\t" << node->m_name;
			PLOGD << "--end seq--";
		}
		PLOGD << "End Sequence";
#endif

		std::vector<Node*> v_rest = Transpiler::get_rest(v_tmp_seq);

		//arrange v_rest to fix arrays dependencies
		Transpiler::arrange_v(v_rest);

#if 0
		PLOGD << "Rest. size = " << v_rest.size();
		for (Node* &node : v_rest)
			PLOGD << "\t" << node->m_name;
		PLOGD << "End Rest";
#endif

		if (v_tmp_seq.size() == 0 && v_rest.size() == 0)
			break;

		PLOGD << "Transpiling sequence...";
		for (std::vector<Node*>& v : v_tmp_seq)
			Transpiler::transpile(v, str_next);
		PLOGD << "Transpiled sequence";

		PLOGD << "Transpiling rest...";
		Transpiler::transpile(v_rest, str_next);
		PLOGD << "Transpiled rest";

		//set for the next loop
		v_start = v_rest;
		pass++;
	}

	//get structs to be freed
	for (std::pair<const std::string, std::string>& e : Transpiler::m_array_init)
	{
		std::string str = fmt::format("  {:s}(&{:s});", e.second, e.first);
		str_free.append(str).append("\n");
	}
	Transpiler::level--;

	//TODO iterate again to add cleanup of memory

	//closing
	str_closing.append("  printf(\"PRESS ENTER TO EXIT\\n\");").append("\n");
	str_closing.append("  getchar();").append("\n");
	str_closing.append("  return 0;").append("\n");
	str_closing.append("}");

	out.append(fmt::format("//Project: {:s}\n", Project::meta.title))
		.append(fmt::format("//Author: {:s}\n", Project::meta.author))
		.append("//C code generated by CodeNect\n\n")
		.append(Templates::s_incl_section)
		.append(str_incl)
		.append(Templates::e_incl_section)

		.append(Templates::s_structs_section)
		.append(str_structs).append("\n")
		.append(Templates::s_structs_section)

		.append(str_entry)
		.append(str_decls).append("\n")
		.append(str_next).append("\n")
		.append(str_free).append("\n")
		.append(str_closing);
	Transpiler::m_temp_names.clear();
}

int Transpiler::compile(void)
{
	if (!Project::has_open_proj)
	{
		Alert::open(ALERT_TYPE::ERROR, "no project is open");
		return RES_FAIL;
	}

	Transpiler::v_declarations.clear();
	Transpiler::m_temp_names.clear();
	Transpiler::m_declared.clear();
	Transpiler::m_array_init.clear();
	tcc_delete(Transpiler::tcc_state);
	Transpiler::init();
	Terminal::is_open = true;
	PLOGI << "Compiling code...";

	Transpiler::recent_temp.clear();
	Transpiler::output_code.clear();
	Transpiler::runnable_code.clear();
	// Transpiler::build_runnable_code(Transpiler::output_code, false);
	Transpiler::build_runnable_code(Transpiler::runnable_code, true);
	PLOGD << Transpiler::runnable_code;

	if (tcc_compile_string(Transpiler::tcc_state, Transpiler::runnable_code.c_str()) == -1)
	{
		Transpiler::error("Could not compile program. Make sure you clear first?");
		return RES_FAIL;
	}

	PLOGI << "Compiled code successfully";
	Transpiler::v_output.push_back({"Compiled code successfully", OUTPUT_TYPE::SUCCESS});
	Terminal::editor.SetText(Transpiler::output_code);
	Transpiler::has_ran = false;
	Transpiler::has_compiled = true;

	return RES_SUCCESS;
}

int Transpiler::run(void)
{
	if (!Project::has_open_proj)
	{
		Alert::open(ALERT_TYPE::ERROR, "no project is open");
		return RES_FAIL;
	}

	if (!Transpiler::has_compiled)
	{
		Alert::open(ALERT_TYPE::ERROR, "must compile first");
		return RES_FAIL;
	}

	Terminal::is_open = true;
	PLOGI << "Running code...";
	Transpiler::v_output.push_back({"Running code...", OUTPUT_TYPE::SUCCESS});
	Transpiler::v_output.push_back({"Saving code...", OUTPUT_TYPE::SUCCESS});
	std::string filename = fmt::format(".__cn_bin_{:s}", Project::meta.title);

	if (!Transpiler::has_ran)
	{
		tcc_output_file(Transpiler::tcc_state, filename.c_str());
		Transpiler::v_output.push_back({"Launching program", OUTPUT_TYPE::SUCCESS});
		Transpiler::has_ran = true;
	}

#if OS_LINUX
	std::string cmd = fmt::format("$TERMINAL -e \"./{:s}\"", filename);
	FILE* p = popen(cmd.c_str(), "r");
	if (p == NULL)
	{
		Transpiler::error("Failed to launch program");
		return RES_FAIL;
	}
	pclose(p);
	Transpiler::v_output.push_back({"Finished running the program", OUTPUT_TYPE::SUCCESS});
#elif OS_WIN
	//TODO
#endif

	return RES_SUCCESS;
}

void Transpiler::clear(void)
{
	Transpiler::v_output.clear();
	Transpiler::v_declarations.clear();
	Transpiler::m_temp_names.clear();
	Transpiler::m_declared.clear();
	Transpiler::m_array_init.clear();
	Transpiler::has_ran = false;
	Transpiler::has_compiled = false;
	tcc_delete(Transpiler::tcc_state);
	Transpiler::init();
}

void Transpiler::save_file(void)
{
	//TODO should handle other type of file? like Lua, Java, Python, etc
	std::string out_filepath = "";
	const char* ext = "c";
	bool res = Filesystem::save_to_file(out_filepath, ext, Transpiler::output_code);

	if (res)
		Alert::open(ALERT_TYPE::SUCCESS, "File saved to " + out_filepath + "." + ext);
	else
		Alert::open(ALERT_TYPE::ERROR, "File cannot be saved to " + out_filepath + "." + ext);
}

void Transpiler::shutdown(void)
{
	PLOGI << "Closing TCC state...";
	tcc_delete(Transpiler::tcc_state);
	PLOGI << "Closed TCC state successfully";
}
}
