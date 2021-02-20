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
	GLFWwindow* m_window;
	ImGuiIO* m_imgui_io;

	void init();
	int init_app();
	void init_window();
	void init_imgui();
	void render_start();
	void render_end();
	void shutdown();
};
}

#endif //APP_HPP
