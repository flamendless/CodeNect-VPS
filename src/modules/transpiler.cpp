#include "modules/transpiler.hpp"

#include <stdio.h>
#include <functional>
#include "plog/Log.h"
#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "node/nodes.hpp"
#include "ui/terminal.hpp"
#include "ui/alert.hpp"
#include "modules//filesystem.hpp"

namespace CodeNect
{
TCCState* Transpiler::tcc_state = nullptr;
std::function<void(char**, int*)> Transpiler::fn;
std::string Transpiler::code = "";
std::string Transpiler::output_code = "";
std::vector<std::pair<std::string, OUTPUT_TYPE>> Transpiler::v_output;
std::vector<std::string> Transpiler::v_declarations;

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

void Transpiler::build_code(void)
{
	std::string output_str = "";
	output_str.append("#include <stdio.h>\n");
	output_str.append("\n");
	output_str.append("int main()\n");
	output_str.append("{\n");
	output_str.append("\tprintf(\"Hello, World!\\n\");\n");
	output_str.append("\treturn 0;\n");
	output_str.append("}");
	Transpiler::output_code = output_str;

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
	Transpiler::build_code();

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
