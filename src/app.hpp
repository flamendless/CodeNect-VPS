#ifndef APP_H
#define APP_H

#include <GLFW/glfw3.h>
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "fmt/core.h"
#include "plog/Init.h"
#include "plog/Log.h"

#if DEBUG_MODE
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#else
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include "config.hpp"
#include "defines.hpp"

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

#endif //APP_H
