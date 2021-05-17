#include "commands.hpp"

#include "plog/Log.h"
#include "ppk_assert.h"

namespace CodeNect
{
std::vector<const Command*> Commands::v_cmd;

void Commands::register_cmd(const Command& cmd)
{
	PPK_ASSERT(cmd.m_tag != Command::TAG::NONE, "Command '%s' must have a function callback set", cmd.m_title.c_str());

	Commands::v_cmd.push_back(&cmd);

	const char* tag;

	switch(cmd.m_tag)
	{
		case Command::TAG::NONE: break;
		case Command::TAG::_VOID: tag = "void"; break;
		case Command::TAG::_INT: tag = "int"; break;
	}

	PLOGI << "Added command: '" << cmd.m_title << "', tag: " << tag;
}

void Commands::shutdown(void)
{
	PLOGI << "Clearing commands...";

	Commands::v_cmd.clear();

	PLOGI << "Cleared commands successfully";
}
}
