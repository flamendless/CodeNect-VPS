#ifndef COMMAND_PALETTE_HPP
#define COMMAND_PALETTE_HPP

#include <functional>
#include <GLFW/glfw3.h>
#include "core/commands.hpp"

namespace CodeNect
{
struct CommandPalette
{
	static int init();
	static void keypress(GLFWwindow* window, int key, int scancode, int mods);
	static void draw();
};
}

#endif //COMMAND_PALETTE_HPP
