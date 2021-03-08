#include "commands.hpp"

#include "plog/Log.h"

namespace CodeNect
{
std::vector<Command> Commands::v_cmd;

void Commands::register_cmd(const Command& cmd)
{
	Commands::v_cmd.push_back(cmd);

	const char* tag;

	switch(cmd.tag)
	{
		case Command::VOID: tag = "void"; break;
		case Command::INT: tag = "int"; break;
	}

	PLOGI << "Added command: '" << cmd.title << "', tag: " << tag;
}
}
