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
#include "core/project.hpp"

namespace CodeNect
{
TCCState* Transpiler::tcc_state = nullptr;
std::function<void(char**, int*)> Transpiler::fn;
std::string Transpiler::code = "";
std::string Transpiler::output_code = "";
std::vector<std::pair<std::string, OUTPUT_TYPE>> Transpiler::v_output;
std::vector<std::string> Transpiler::v_declarations;
int Transpiler::level = 0;

int Transpiler::init(void)
{
	Transpiler::tcc_state = tcc_new();

	if (!Transpiler::tcc_state)
	{
		PLOGE << "Could not create TCC state";
		return RES_FAIL;
	}

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

		.append(Templates::s_declaration_section)
		.append(str_decls)
		.append(Templates::s_declaration_section)

		.append(str_closing);

	Transpiler::output_code = str_final;
}

void Transpiler::build_out_code(void)
{
	std::string real_str = "";
	real_str.append("#include <tcclib.h>");
	real_str.append("void main(char** chars, int* size)");
	real_str.append("{");
	real_str.append("\tprintf(\"Hello, World!\\n\");");
	real_str.append("\tprintf(\"-from TCC\\n\");");
	real_str.append("strcpy(chars[0], \"Hello, World!\");");
	real_str.append("strcpy(chars[1], \"-from TCC\");");
	real_str.append("strcpy(chars[2], \"-from TCC\");");
	real_str.append("*size = 3;");
	real_str.append("}");
	Transpiler::code = real_str;
}

int Transpiler::compile(void)
{
	tcc_delete(Transpiler::tcc_state);
	Transpiler::init();
	Terminal::is_open = true;
	PLOGI << "Compiling code...";
	Transpiler::build_runnable_code();
	Transpiler::build_out_code();

	if (tcc_compile_string(Transpiler::tcc_state, Transpiler::code.c_str()) == -1)
	{
		PLOGE << "Could not compile program.\nMake sure you clear first";
		Transpiler::v_output.push_back({"Could not compile program.\nMake sure you clear first", OUTPUT_TYPE::ERROR});
		return RES_FAIL;
	}

	if (tcc_relocate(Transpiler::tcc_state, TCC_RELOCATE_AUTO) < 0)
	{
		PLOGE << "Could not relocate";
		Transpiler::v_output.push_back({"Could not relocate program", OUTPUT_TYPE::ERROR});
		return RES_FAIL;
	}

	PLOGI << "Compiled code successfully";
	Transpiler::v_output.push_back({"Compiled code successfully", OUTPUT_TYPE::SUCCESS});

	Terminal::editor.SetText(Transpiler::output_code);

	return RES_SUCCESS;
}

int Transpiler::run(void)
{
	Terminal::is_open = true;
	PLOGI << "Running code...";
	Transpiler::v_output.push_back({"Running code...", OUTPUT_TYPE::SUCCESS});
	Transpiler::fn = (void(*)(char**, int*))tcc_get_symbol(Transpiler::tcc_state, "main");

	if (!fn)
	{
		PLOGE << "Could not get entry point from program.\nMake sure you have compiled first";
		Transpiler::v_output.push_back({"Could not get entry point from program.\nMake sure you have compiled first", OUTPUT_TYPE::ERROR});
		return RES_FAIL;
	}

	//TODO
	//128 should depende on the number of node_print?
	char** ret = (char**)malloc(128 * sizeof(char*));
	for (int y = 0; y < 128; y++)
		for (int x = 0; x < 128; x++)
			ret[y] = (char*)malloc(128 * sizeof(char));

	int size = 0;
	Transpiler::fn(ret, &size);

	PLOGV << "Used size: " << size;
	for (int i = 0; i < size; i++)
	{
		PLOGV << "Output: " << ret[i];
		Transpiler::v_output.push_back({ret[i], OUTPUT_TYPE::NORMAL});
	}

	delete ret;

	PLOGI << "Ran code successfully";
	Transpiler::v_output.push_back({"Ran code successfully", OUTPUT_TYPE::SUCCESS});

	return RES_SUCCESS;
}

void Transpiler::clear(void)
{
	Transpiler::v_output.clear();
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
