#include "modules/input.hpp"

#include "plog/Log.h"

namespace CodeNect
{
std::vector<keycallback_t> Input::v_keypresses;
// std::vector<keycallback_t> Input::v_keyreleases;

void Input::register_keypress(keycallback_t fn)
{
	PLOGD << "Registered keypress callback";

	Input::v_keypresses.push_back(fn);
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		for (keycallback_t cb : Input::v_keypresses)
		{
			cb(window, key, scancode, mods);
		}
	}
	// else if (action == GLFW_RELEASE)
	// {
	// 	for (keycallback_t cb : Input::v_keyreleases)
	// 	{
	// 		cb(window, key, scancode, mods);
	// 	}
	// }
}
}
