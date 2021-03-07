#ifndef COMMAND_PALETTE_HPP
#define COMMAND_PALETTE_HPP

#include <functional>
#include <GLFW/glfw3.h>

namespace CodeNect
{
struct CommandPalette
{
	static int init();
	static void keypress(GLFWwindow* window, int key, int scancode, int mods);
};
}

#endif //COMMAND_PALETTE_HPP
