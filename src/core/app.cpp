#include "core/app.hpp"

#include <stdio.h>
#include "stb_image.h"
#include "IconsFontAwesome5.h"
#include "fmt/core.h"
#include "plog/Init.h"
#include "plog/Log.h"

#ifndef OS_WIN
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#else
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include "core/commands.hpp"
#include "core/config.hpp"
#include "core/defines.hpp"
#include "modules/filesystem.hpp"
#include "modules/input.hpp"
#include "core/project.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
GLFWwindow* App::window;

void glfw_error_callback(int error, const char* desc)
{
	std::string log = fmt::format("Glfw error %d: %s", error, desc);
	PLOGE << log;
}

void glfw_close_callback(GLFWwindow* window)
{
	if (Project::has_unsaved_changes())
		glfwSetWindowShouldClose(window, GL_FALSE);
	else
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void App::register_commands(void)
{
	Command* cmd_quit = new Command("Quit CodeNect", "quit the software", ICON_FA_TIMES_CIRCLE);
	cmd_quit->set_fn(App::quit_app);
	cmd_quit->m_close_command_palette = true;

	Commands::register_cmd(*cmd_quit);
}

void App::quit_app()
{
	if (!Project::has_unsaved_changes())
		glfwSetWindowShouldClose(App::window, GL_TRUE);
}

void App::init(void)
{
#ifndef OS_WIN
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

	PLOGI << "Setting window icon...";
	GLFWimage images[1];
	images[0].pixels = Filesystem::load_texture_from_file("assets/logo.png", images[0]);
	glfwSetWindowIcon(App::window, 1, images);
	stbi_image_free(images[0].pixels);
	PLOGI << "Set window icon successfully";

	return RES_SUCCESS;
}

void App::init_window(void)
{
	if (CodeNect::Config::fullscreen)
	{
		PLOGI << "Using monitor's fullscreen resolution...";
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		Config::win_width = mode->width;
		Config::win_height = mode->height;
	}
	App::window = glfwCreateWindow(CodeNect::Config::win_width, CodeNect::Config::win_height,
		"CodeNect", NULL, NULL);

	glfwSetWindowCloseCallback(App::window, glfw_close_callback);
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

	PLOGI << "Application closed successfully";
}
}
