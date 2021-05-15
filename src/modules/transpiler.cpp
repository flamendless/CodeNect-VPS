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
#include "node/node_branch.hpp"
#include "node/node_loop.hpp"
#include "node/node_for.hpp"
#include "core/project.hpp"
#include "core/utils.hpp"
#include "modules/debugger.hpp"

namespace CodeNect
{
TCCState* Transpiler::tcc_state = nullptr;
std::string Transpiler::output_code = "";
std::string Transpiler::runnable_code = "";
std::vector<MessageInfo> Transpiler::v_output;
std::vector<std::string> Transpiler::v_declarations;
std::map<std::string, bool> Transpiler::m_temp_names;
std::map<std::string, bool> Transpiler::m_declared;
std::map<std::string, std::string> Transpiler::m_array_init;
int Transpiler::level = 0;
bool Transpiler::has_ran = false;
bool Transpiler::has_compiled = false;
std::string Transpiler::recent_temp = "";
int Transpiler::n_transpiled = 0;
std::vector<State> Transpiler::v_states;
std::vector<NodeBranch*> Transpiler::v_finished_branches;

bool has_warning_added = false;
std::array<std::string, 2> ignored_warnings = {
	"warning: assignment discards qualifiers from pointer target type",
	"warning: assignment from incompatible pointer type",
};

void Transpiler::handle_error(void* opaque, const char* msg)
{
	OUTPUT_TYPE out_type = OUTPUT_TYPE::NORMAL;
	std::string str_msg = msg;

	//check if it's warning or error
	if (str_msg.find("warning") != std::string::npos)
		out_type = OUTPUT_TYPE::WARNING;

	//check for ignored messages
	for (std::string& str : ignored_warnings)
	{
		if (str_msg.find(str) != std::string::npos)
		{
			PLOGW << "ignored warning: " << msg;
			return;
		}
	}

	Transpiler::add_message(std::move(msg), out_type);
	if (!has_warning_added)
	{
		Transpiler::add_message(std::move("You can see error markers in the code tab"), out_type);
		has_warning_added = true;
	}

	//get the line number (not perfect)
	int i = 0;
	for (i = 0; i < str_msg.length(); i++)
	{
		if (isdigit(str_msg[i]))
			break;
	}

	str_msg = str_msg.substr(i, str_msg.length() - i);
	int ln = std::atoi(str_msg.c_str());
	TextEditor::ErrorMarkers markers;
	markers.insert(std::make_pair<int, std::string>(std::move(ln), msg));
	Terminal::editor.SetErrorMarkers(markers);
}

int Transpiler::init(void)
{
	Transpiler::level = 0;
	Transpiler::tcc_state = tcc_new();

	if (!Transpiler::tcc_state)
	{
		PLOGE << "Could not create TCC state";
		return RES_FAIL;
	}

	tcc_set_error_func(Transpiler::tcc_state, stderr, Transpiler::handle_error);
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

void Transpiler::add_message(const std::string& msg, OUTPUT_TYPE type, Node* node, DOC_ID doc_id)
{
	PLOGD << msg;
	MessageInfo info;
	info.m_msg = std::move(msg);
	info.m_type = type;
	info.m_node = node;
	info.m_doc_id = doc_id;
	Transpiler::v_output.push_back(std::move(info));
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
	bool has_string = false;

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
		NodeString* node_str = dynamic_cast<NodeString*>(*it);

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

		if (!has_string && node_str)
		{
			str_structs.append(Templates::string_methods);
			has_string = true;
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
		++Transpiler::n_transpiled;

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
				Transpiler::add_message(std::move(warning), OUTPUT_TYPE::WARNING, node);
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
			++Transpiler::n_transpiled;
		}
	}
}

std::vector<std::vector<Node*>> Transpiler::get_v_sequence(State* state)
{
	//get the independent sequence/chain of nodes
	std::vector<Node*>& v_entry = state->v_rest;
	std::vector<std::vector<Node*>> v_out;
	for (std::vector<Node*>::iterator it = v_entry.begin();
		it != v_entry.end();
		it++)
	{
		Node* node = *it;
		std::vector<Node*> v_seq = Transpiler::get_sequence(node, state);

		if (v_seq.size() == 0)
			v_seq.push_back(node);
		v_out.push_back(v_seq);
	}

	std::vector<std::vector<Node*>> v_final;
	for (std::vector<Node*>& v : v_out)
	{
		bool branch_found = false;
		std::vector<Node*> v_temp;
		for (std::vector<Node*>::iterator it = v.begin();
			it != v.end();
			it++)
		{
			Node* node = static_cast<Node*>(*it);
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
			if (node_branch)
			{
				branch_found = true;
				std::vector<Node*> v_new;
				v_new.push_back(node_branch);
				v_final.push_back(v_new);
			}
			else
				v_temp.push_back(node);
		}

		if (branch_found)
			v_final.push_back(v_temp);
		else
			v_final.push_back(v);
	}
	return v_final;
}

std::vector<Node*> Transpiler::get_sequence(Node* start_node, State* current_state)
{
	std::vector<Node*> v;
	std::vector<Node*> v_last;
	for (const Connection& connection : start_node->m_connections)
	{
		//in_node is the next node to check for sequence
		Node* in_node = static_cast<Node*>(connection.in_node);
		NodeBranch* node_branch = dynamic_cast<NodeBranch*>(in_node);
		if (node_branch)
		{
			//we basically want to stop a sequence once we reach a NodeBranch
			PLOGD << "found NodeBranch " << node_branch->m_name << ". Stopping";
			v_last.push_back(node_branch);
			continue;
		}

		if (in_node != start_node)
		{
			unsigned int count = Nodes::count_node_dep(in_node);
			if (count == 1)
			{
				if (current_state->node_branch)
				{
					//start_node is for sure a NodeBranch
					//in_node is the immediate of start_branch
					for (const Connection& connection : start_node->m_connections)
					{
						Node* nb_in_node = static_cast<Node*>(connection.in_node);
						Node* nb_out_node = static_cast<Node*>(connection.out_node);
						if (nb_in_node == in_node && nb_out_node == start_node)
						{
							const char* slot = connection.out_slot;
							bool is_true = std::strcmp(slot, "TRUE") == 0;
							bool is_false = std::strcmp(slot, "FALSE") == 0;

							if ((!current_state->is_in_else && is_true) ||
								(current_state->is_in_else && is_false))
							{
								v.push_back(in_node);
								std::vector<Node*> v2 = Transpiler::get_sequence(in_node, current_state);
								v.insert(v.end(), v2.begin(), v2.end());
							}
						}
					}
				}
				else
				{
					v.push_back(in_node);
					std::vector<Node*> v2 = Transpiler::get_sequence(in_node, current_state);
					v.insert(v.end(), v2.begin(), v2.end());
				}
			}
		}
	}
	v.insert(v.end(), v_last.begin(), v_last.end());
	return v;
}

std::vector<Node*> Transpiler::get_rest(State* current_state)
{
	//get the rest to be transpiled using the last node in the sequence/chain
	std::vector<Node*> v_out;
	for (std::vector<Node*>& v : current_state->v_seq)
	{
		for (Node* &node : v)
		{
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
			if (node_branch)
			{
				if (!current_state->node_branch)
					continue;
			}

			if (current_state->node_branch)
			{
				if (node_branch)
					continue;
			}

			for (const Connection& connection : node->m_connections)
			{
				Node* in_node = static_cast<Node*>(connection.in_node);
				if (in_node != node)
				{
					if (std::find(v_out.begin(), v_out.end(), in_node) != v_out.end())
						continue;
					v_out.push_back(in_node);
				}
			}
		}
	}
	return v_out;
}

void Transpiler::arrange_v(std::vector<Node*>& v)
{
	std::vector<Node*> temp_v;
	std::vector<Node*>::iterator it = v.begin();
	while (it != v.end())
	{
		Node* node = *it;
		NodeArray* node_array = dynamic_cast<NodeArray*>(node);

		if (node_array)
		{
			temp_v.push_back(node_array);
			it = v.erase(it);
		}
		else
			++it;
	}
	for (Node* &node : temp_v)
		v.push_back(node);
}

int Transpiler::compile(void)
{
	if (!Project::has_open_proj)
	{
		Alert::open(ALERT_TYPE::ERROR, "no project is open");
		return RES_FAIL;
	}

	tcc_delete(Transpiler::tcc_state);
	Transpiler::init();
	Terminal::is_open = true;
	PLOGI << "Compiling code...";

	Transpiler::recent_temp.clear();
	Transpiler::output_code.clear();
	Transpiler::runnable_code.clear();

	Transpiler::build_runnable_code(Transpiler::runnable_code, true);
	// Transpiler::build_runnable_code(Transpiler::output_code, false);

	PLOGD << Transpiler::runnable_code;

	if (tcc_compile_string(Transpiler::tcc_state, Transpiler::runnable_code.c_str()) == -1)
	{
		Transpiler::add_message(std::move("Could not compile program. Make sure you clear first?"), OUTPUT_TYPE::ERROR);
		Terminal::editor.SetText(Transpiler::runnable_code);
		return RES_FAIL;
	}

	Terminal::editor.SetText(Transpiler::output_code);
	PLOGI << "Compiled code successfully";
	Transpiler::add_message(std::move("Compiled code successfully"));
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
	Transpiler::add_message(std::move("Running code..."));
	Transpiler::add_message(std::move("Saving code..."));
	std::string filename = fmt::format(".__cn_bin_{:s}", Project::meta.title);

	if (!Transpiler::has_ran)
	{
		tcc_output_file(Transpiler::tcc_state, filename.c_str());
		Transpiler::add_message(std::move("Launching program"));
		Transpiler::has_ran = true;
	}

#if OS_LINUX
	std::string cmd = fmt::format("$TERMINAL -e \"./{:s}\"", filename);
	FILE* p = popen(cmd.c_str(), "r");
	if (p == NULL)
	{
		Transpiler::add_message(std::move("Failed to launch program"), OUTPUT_TYPE::ERROR);
		return RES_FAIL;
	}
	pclose(p);
	Transpiler::add_message(std::move("Finished running the program"));
#elif OS_WIN
	//TODO
#endif

	return RES_SUCCESS;
}

void Transpiler::clear(void)
{
	has_warning_added = false;
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
