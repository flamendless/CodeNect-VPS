#include "modules/transpiler.hpp"

#include <stdio.h>
#include <functional>
#include "plog/Log.h"
#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
TCCState* Transpiler::tcc_state = nullptr;
std::function<const char*(void)> Transpiler::fn;

//test
std::string Transpiler::code =
"#include <tcclib.h>\n"
"const char* test(void)\n"
"{\n"
"    printf(\"Hello, World!\n\");\n"
"    return \"Hello, World! -from TCC\";\n"
"}";
std::vector<std::string> Transpiler::v_output;

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

int Transpiler::compile(void)
{
	PLOGI << "Compiling code...";
	if (tcc_compile_string(Transpiler::tcc_state, Transpiler::code.c_str()) == -1)
	{
		PLOGE << "Could not compile program";
		Alert::open(ALERT_TYPE::ERROR, "Could not compile program");
		return RES_FAIL;
	}

	if (tcc_relocate(Transpiler::tcc_state, TCC_RELOCATE_AUTO) < 0)
	{
		PLOGE << "Could not relocate";
		Alert::open(ALERT_TYPE::ERROR, "Could not compile program");
		return RES_FAIL;
	}

	PLOGI << "Compiled code successfully";
	Alert::open(ALERT_TYPE::SUCCESS, "Compiled code successfully");

	return RES_SUCCESS;
}

int Transpiler::run(void)
{
	PLOGI << "Running code...";
	Transpiler::fn = (const char*(*)(void))tcc_get_symbol(Transpiler::tcc_state, "test");

	if (!fn)
	{
		PLOGE << "Could not get entry point from program";
		Alert::open(ALERT_TYPE::ERROR, "Could not get entry point from program");
		return RES_FAIL;
	}

	const char* ret = Transpiler::fn();
	PLOGV << "Output: " << ret;
	Transpiler::v_output.push_back(ret);

	PLOGI << "Ran code successfully";

	return RES_SUCCESS;
}

void Transpiler::clear(void)
{
	Transpiler::v_output.clear();
}

void Transpiler::shutdown(void)
{
	PLOGI << "Closing TCC state...";
	tcc_delete(Transpiler::tcc_state);
	PLOGI << "Closed TCC state successfully";
}
}
