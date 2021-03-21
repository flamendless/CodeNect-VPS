#include "core/app.hpp"

#include <stdio.h>
#include "IconsFontAwesome5.h"
#include "fmt/core.h"
#include "plog/Init.h"
#include "plog/Log.h"

#if DEBUG_MODE
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#else
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include "core/commands.hpp"
#include "core/config.hpp"
#include "core/defines.hpp"
#include "modules/input.hpp"

namespace CodeNect
{
GLFWwindow* App::window;

void glfw_error_callback(int error, const char* desc)
{
	std::string log = fmt::format("Glfw error %d: %s", error, desc);
	PLOGE << log;
}

void App::register_commands(void)
{
	Command* cmd_quit = new Command("Quit CodeNect", "quit the software", ICON_FA_TIMES_CIRCLE);
	cmd_quit->set_fn(App::quit_app);

	Commands::register_cmd(*cmd_quit);
}

void App::quit_app()
{
	glfwSetWindowShouldClose(App::window, GL_TRUE);
}

void App::init(void)
{
#if DEBUG_MODE
	static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
	plog::init(plog::verbose, &console_appender);
#else
	plog::init(plog::verbose, "log.txt");
#endif
}

int App::init_app(void)
{
	PLOGI << "Application starting...";

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
		return RES_FAIL;

	App::init_window();
	App::init_imgui();

	return RES_SUCCESS;
}

void App::init_window(void)
{
	App::window = glfwCreateWindow(CodeNect::Config::win_width, CodeNect::Config::win_height,
		CodeNect::Config::app_title.c_str(), NULL, NULL);

	glfwSetKeyCallback(App::window, Input::key_callback);
	glfwMakeContextCurrent(App::window);
	glfwSwapInterval(CodeNect::Config::vsync);
}

void App::init_imgui(void)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO* imgui_io = &ImGui::GetIO(); (void)imgui_io;

	Config::set_style(-1);

	ImGui_ImplGlfw_InitForOpenGL(App::window, true);
	ImGui_ImplOpenGL2_Init();
}

void App::render_start(void)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void App::render_end(void)
{
	const ImVec4& clear_color = CodeNect::Config::clear_color;
	int display_w, display_h;

	ImGui::Render();
	glfwGetFramebufferSize(App::window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(App::window);
	glfwSwapBuffers(App::window);
}

void App::shutdown(void)
{
	PLOGI << "Application closing...";

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(App::window);
	glfwTerminate();
}
}
