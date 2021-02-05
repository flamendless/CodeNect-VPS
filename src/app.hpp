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
	static GLFWwindow* window;
	static ImGuiIO* imgui_io;

	static void glfw_error_callback(int error, const char* desc);
	static void init();
	static int init_app();
	static void init_window();
	static void init_imgui();
	static void render_start();
	static void render_end();
	static void shutdown();
};
}

#endif //APP_H
