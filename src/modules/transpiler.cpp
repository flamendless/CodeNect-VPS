#include "modules/transpiler.hpp"
#include "modules/node_to_code.hpp"

#include <stdio.h>
#include <functional>
#include <sys/wait.h>
#include <sys/mman.h>
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
#include "core/project.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
TCCState* Transpiler::tcc_state = nullptr;
std::string Transpiler::code = "";
std::string Transpiler::output_code = "";
std::vector<std::pair<std::string, OUTPUT_TYPE>> Transpiler::v_output;
std::vector<std::string> Transpiler::v_declarations;
int Transpiler::level = 0;
bool Transpiler::has_ran = false;
bool Transpiler::has_compiled = false;

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

//fill the string for includes and structs
void Transpiler::set_pre_entry(std::string& str_incl, std::string& str_structs)
{
	//includes
	bool has_io = false;
	bool has_math = false;
	bool has_stdlib = false;
	bool has_d_a_bool = false;
	bool has_d_a_int = false;
	bool has_d_a_float = false;
	bool has_d_a_double = false;
	bool has_d_a_str = false;
	bool has_prompt = false;
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodePrint* node_print = dynamic_cast<NodePrint*>(*it);
		NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(*it);
		NodeMath* node_math = dynamic_cast<NodeMath*>(*it);
		NodeArray* node_array = dynamic_cast<NodeArray*>(*it);

		if (has_io && has_math)
			break;

		if (!has_io && (node_print || node_prompt))
		{
			str_incl.append(Templates::incl_stdio);
			has_io = true;
			continue;
		}

		if (!has_math && node_math)
		{
			str_incl.append(Templates::incl_math);
			has_math = true;
			continue;
		}

		if (!has_prompt && node_prompt)
		{
			str_structs.append(Templates::struct_prompt);
			has_prompt = true;
			continue;
		}

		if (node_array && node_array->m_array == +NODE_ARRAY::DYNAMIC)
		{
			if (!has_stdlib)
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

//NOTE here we do multiple appends instead of one append with \n at the end,
//it is not efficient but we trade performance for readability
void Transpiler::build_runnable_code(void)
{
	std::string str_final = "";
	std::string str_incl = "";
	std::string str_structs = "";
	std::string str_entry = "";
	std::string str_decls = "";
	std::string str_closing = "";
	Transpiler::set_pre_entry(str_incl, str_structs);

	//entry point
	str_entry.append("int main()").append("\n");
	str_entry.append("{").append("\n");

	//store
	std::vector<Node*> v_decls;

	//find all nodes that do NOT have any LHS, this means that they are for declarations
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = *it;
		if (Nodes::check_if_no_lhs(node))
			v_decls.push_back(node);
	}

	Transpiler::level++;
	//the following are only possible to be nodes without any LHS
	for (std::vector<Node*>::iterator it = v_decls.begin();
		it != v_decls.end();
		it++)
	{
		Node* node = *it;
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(node);
		NodeArray* node_array = dynamic_cast<NodeArray*>(node);
		NodePrint* node_print = dynamic_cast<NodePrint*>(node);
		NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(node);

		if (node_var)
		{
			str_decls.append(NodeToCode::comment(node));
			str_decls.append(NodeToCode::node_var(node_var));
		}
		else if (node_array)
		{
			str_decls.append(NodeToCode::comment(node));
			str_decls.append(NodeToCode::node_array(node_array));
		}
		else if (node_print)
		{
			str_decls.append(NodeToCode::comment(node));
			str_decls.append(NodeToCode::node_print(node_print));
		}
		else if (node_prompt)
		{
			str_decls.append(NodeToCode::comment(node));
			str_decls.append(NodeToCode::node_prompt(node_prompt));
		}
	}
	Transpiler::level--;

	//closing
	str_closing.append("\n  return 0;").append("\n");
	str_closing.append("}");

	str_final
		.append(fmt::format("//Project: {:s}\n", Project::meta.title))
		.append(fmt::format("//Author: {:s}\n", Project::meta.author))
		.append("//C code generated by CodeNect\n\n")
		.append(Templates::s_incl_section)
		.append(str_incl).append("\n")
		.append(Templates::e_incl_section)

		.append(Templates::s_structs_section)
		.append(str_structs).append("\n")
		.append(Templates::s_structs_section)

		.append(str_entry)
		.append(str_decls).append("\n")
		.append(str_closing);

	Transpiler::output_code = str_final;
}

void Transpiler::build_out_code(void)
{
	std::string real_str = "";
	real_str.append("#include <tcclib.h>\n");
	real_str.append("int main()\n");
	real_str.append("{\n");
	real_str.append("  printf(\"Hello, World!\\n\");");
	real_str.append("  printf(\"Hi there\\n\");");
	real_str.append("  printf(\"-from TCC to CodeNect\\n\");");
	// real_str.append("  char* buffer;");
	// real_str.append("  size_t size = 32;");
	// real_str.append("  printf(\"enter input: \");");
	// real_str.append("  getline(&buffer, &size, stdin);");
	// real_str.append("  printf(\"user input is: %s\\n\", buffer);");
	real_str.append("  getchar();");
	real_str.append("  return 0;");
	real_str.append("}");
	Transpiler::code = real_str;
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
	Transpiler::build_runnable_code();
	Transpiler::build_out_code();

	if (tcc_compile_string(Transpiler::tcc_state, Transpiler::code.c_str()) == -1)
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
