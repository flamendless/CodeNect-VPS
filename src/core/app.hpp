#ifndef APP_HPP
#define APP_HPP

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

namespace CodeNect
{
struct App
{
	static GLFWwindow* window;

	void init(void);
	int init_app(void);
	void init_window(void);
	void init_imgui(void);
	void render_start(void);
	void render_end(void);
	void shutdown(void);
};
}

#endif //APP_HPP
