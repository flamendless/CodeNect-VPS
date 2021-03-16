#ifndef _APP_HPP
#define _APP_HPP

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

namespace CodeNect
{
struct App
{
	static GLFWwindow* window;

	static void register_commands(void);
	static void quit_app(void);

	void init(void);
	int init_app(void);
	void init_window(void);
	void init_imgui(void);
	void render_start(void);
	void render_end(void);
	void shutdown(void);
};
}

#endif //_APP_HPP
