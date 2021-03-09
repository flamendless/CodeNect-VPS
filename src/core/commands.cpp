#include "commands.hpp"

#include "plog/Log.h"
#include "ppk_assert.h"

namespace CodeNect
{
std::vector<Command> Commands::v_cmd;

void Commands::register_cmd(const Command& cmd)
{
	PPK_ASSERT(cmd.tag != Command::NONE, "Command '%s' must have a function callback set", cmd.title.c_str());

	Commands::v_cmd.push_back(cmd);

	const char* tag;

	switch(cmd.tag)
	{
		case Command::NONE: break;
		case Command::VOID: tag = "void"; break;
		case Command::INT: tag = "int"; break;
	}

	PLOGI << "Added command: '" << cmd.title << "', tag: " << tag;
}
}
