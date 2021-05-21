#include "modules/transpiler.hpp"
#include "modules/node_to_code.hpp"

#include <algorithm>
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
#include "core/config.hpp"
#include "modules/debugger.hpp"
#include "modules/filesystem.hpp"
#include "modules/assessments.hpp"

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
// std::vector<std::string> Transpiler::v_lines;
std::vector<std::string> Transpiler::v_printed;

bool has_warning_added = false;
std::array<std::string, 2> ignored_warnings = {
	"warning: assignment discards qualifiers from pointer target type",
	"warning: assignment from incompatible pointer type",
};

void cn_printf(const char* fmt, ...)
{
	char buffer[256];
	va_list args;
    va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	printf("%s", buffer);

	//sanitize string
	std::string str_buffer = buffer;
	str_buffer.erase(str_buffer.find_last_not_of("\n\r\t") + 1);
	Transpiler::v_printed.push_back(str_buffer);
}

char* cn_fgets(char* str, int num, FILE* s)
{
	char* ret = fgets(str, num, s);

	//sanitize
	std::string str_buffer = str;
	str_buffer.erase(str_buffer.find_last_not_of("\n\r\t") + 1);
	std::string str_prev = Transpiler::v_printed.back();
	Transpiler::v_printed.pop_back();
	Transpiler::v_printed.push_back(str_prev + str_buffer);
	return ret;
}

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

#ifdef OS_LINUX
	tcc_set_lib_path(Transpiler::tcc_state, "./");
#endif
	tcc_add_library_path(Transpiler::tcc_state, "./tcc");
	tcc_add_include_path(Transpiler::tcc_state, "./tcc");
	tcc_set_error_func(Transpiler::tcc_state, stderr, Transpiler::handle_error);
	// tcc_set_output_type(Transpiler::tcc_state, TCC_OUTPUT_EXE);
	tcc_set_output_type(Transpiler::tcc_state, TCC_OUTPUT_MEMORY);

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
		case NODE_KIND::LOOP: valid = true; break;
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

int Transpiler::compile(void)
{
	if (!Project::has_open_proj)
	{
		Alert::open(ALERT_TYPE::ERR, "no project is open");
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
	Transpiler::build_runnable_code(Transpiler::output_code, false);

	PLOGD << "Runnable code:\n" << Transpiler::runnable_code;
	PLOGD << "Output code:\n" << Transpiler::output_code;

	if (tcc_compile_string(Transpiler::tcc_state, Transpiler::runnable_code.c_str()) == -1)
	{
		Transpiler::add_message(std::move("Could not compile program. Make sure you clear first?"), OUTPUT_TYPE::ERR);
		Terminal::editor.SetText(Transpiler::runnable_code);
		return RES_FAIL;
	}

	tcc_add_symbol(Transpiler::tcc_state, "cn_printf", (void*)cn_printf);
	tcc_add_symbol(Transpiler::tcc_state, "cn_fgets", (void*)cn_fgets);
	if (tcc_relocate(Transpiler::tcc_state, TCC_RELOCATE_AUTO) < 0)
	{
		PLOGE << "Could not relocate";
		return RES_FAIL;
	}

	//replace occurences of cn_printf with printf
	size_t index = 0;
	std::string needle = "cn_printf";
	std::string new_str = "printf";
	while ((index = Transpiler::output_code.find(needle, index)) != std::string::npos)
	{
		Transpiler::output_code.replace(index, needle.length(), new_str);
		index += new_str.length();
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
		Alert::open(ALERT_TYPE::ERR, "no project is open");
		return RES_FAIL;
	}

	if (!Transpiler::has_compiled)
	{
		Alert::open(ALERT_TYPE::ERR, "must compile first");
		return RES_FAIL;
	}

	Terminal::is_open = true;
	PLOGI << "Running code...";
	Transpiler::add_message(std::move("Running code..."));

	// std::filesystem::path od = Filesystem::Paths::out_dir;
	// od.append(Project::meta.file_bin);
	// std::string filename = od.string();

	if (!Transpiler::has_ran)
	{
		// tcc_output_file(Transpiler::tcc_state, filename.c_str());
		Transpiler::add_message(std::move("Launching program..."));
		Transpiler::has_ran = true;
	}

	int(*func)(void);
	func = (int(*)(void))tcc_get_symbol(Transpiler::tcc_state, "main");
	if (!func)
	{
		PLOGE << "unable to get program's entry point";
		return RES_FAIL;
	}

	func();

	// if (Transpiler::run_cmd(filename) == RES_FAIL)
	// 	return RES_FAIL;

	Transpiler::add_message(std::move("Finished running the program"));
	return RES_SUCCESS;
}

// int Transpiler::run_cmd(std::string& filename)
// {
// 	std::filesystem::path od = Filesystem::Paths::out_dir;
// 	od.append(Project::meta.file_stdout);
// 	std::string out_filename = od.string();
//
// #ifdef OS_LINUX
// 	bool res = false;
// 	std::string cmd_linux = fmt::format("{:s} -e \"script -q -c './{:s}' {:s}\"",
// 			Config::terminal, filename, out_filename);
// 	Utils::get_stdout_from_cmd(cmd_linux, res);
// 	if (!res)
// 	{
// 		Transpiler::add_message(std::move("Failed to launch program"), OUTPUT_TYPE::ERR);
// 		return RES_FAIL;
// 	}
// 	else
// 	{
// 		Transpiler::v_lines = Filesystem::parse_stdout(out_filename);
// 		if (v_lines.size() == 0)
// 		{
// 			Transpiler::add_message(std::move("Can't open output file"), OUTPUT_TYPE::ERR);
// 			return RES_FAIL;
// 		}
//
// 		//remove:
// 		//1st line - script start
// 		//2nd to last line - prompt
// 		//last line - script end
// 		v_lines.erase(v_lines.begin());
// 		v_lines.erase(v_lines.end() - 3, v_lines.end());
//
// 		Transpiler::add_message(std::move("Total lines: " + std::to_string(v_lines.size())), OUTPUT_TYPE::STDOUT);
// 		Transpiler::add_message(std::move("-----OUTPUT-----"), OUTPUT_TYPE::STDOUT);
// 		for (unsigned long i = 0; i < v_lines.size(); i++)
// 			Transpiler::add_message(v_lines[i], OUTPUT_TYPE::STDOUT);
// 		Transpiler::add_message(std::move("----------------"), OUTPUT_TYPE::STDOUT);
// 	}
//
// #elif OS_WIN
// 	ShellExecute(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWDEFAULT);
// #endif
//
// 	return RES_SUCCESS;
// }

void Transpiler::clear(void)
{
	has_warning_added = false;
	Transpiler::v_printed.clear();
	Transpiler::v_output.clear();
	Transpiler::v_declarations.clear();
	Transpiler::m_temp_names.clear();
	Transpiler::m_declared.clear();
	Transpiler::m_array_init.clear();
	// Transpiler::v_lines.clear();
	Transpiler::has_ran = false;
	Transpiler::has_compiled = false;
	Assessments::v_results.clear();
	tcc_delete(Transpiler::tcc_state);
	Transpiler::init();
}

void Transpiler::save_file(void)
{
	std::string out_filepath = "";
	const char* ext = "c";
	bool res = Filesystem::save_to_file(out_filepath, ext, Transpiler::output_code);

	if (res)
		Alert::open(ALERT_TYPE::SUCCESS, "File saved to " + out_filepath + "." + ext);
	else
		Alert::open(ALERT_TYPE::ERR, "File cannot be saved to " + out_filepath + "." + ext);
}

void Transpiler::shutdown(void)
{
	PLOGI << "Closing TCC state...";
	tcc_delete(Transpiler::tcc_state);
	PLOGI << "Closed TCC state successfully";
}
}
