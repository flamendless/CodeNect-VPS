#ifndef INPUT_HPP
#define INPUT_HPP

#include <functional>
#include <GLFW/glfw3.h>
#include <vector>

namespace CodeNect
{
typedef std::function<void(GLFWwindow* window, int key, int scancode, int mods)> keycallback_t;

struct Input
{
	static std::vector<keycallback_t> v_keypresses;
	// static std::vector<keycallback_t> v_keyreleases;

	static void register_keypress(keycallback_t fn);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
}

#endif //INPUT_HPP