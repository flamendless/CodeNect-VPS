#include "ui/command_palette.hpp"

#include "plog/Log.h"
#include "modules/input.hpp"
#include "core/defines.hpp"
#include "core/commands.hpp"

namespace CodeNect
{
int CommandPalette::init()
{
	Input::register_keypress(CommandPalette::keypress);

	return RES_SUCCESS;
}

void CommandPalette::keypress(GLFWwindow* window, int key, int scancode, int mods)
{
	if (key == GLFW_KEY_P && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
	{
		PLOGV << "Command Palette launched";

		for (Command cmd : Commands::v_cmd)
		{
			PLOGD << "Command: " << cmd.title;
		}
	}
}

void CommandPalette::draw()
{
}
}
