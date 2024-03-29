#ifndef _INPUT_HPP
#define _INPUT_HPP

#include <functional>
#include <GLFW/glfw3.h>
#include <vector>

namespace CodeNect
{
typedef std::function<bool(int key, int scancode, int mods)> keycallback_t;

struct Input
{
	static std::vector<keycallback_t> v_keypresses;
	static std::vector<keycallback_t> v_keyreleases;

	Input() = delete;
	static void register_key_event(keycallback_t fn, bool press = true);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
}

#endif //_INPUT_HPP
