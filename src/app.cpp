#include "app.hpp"

namespace CodeNect
{
GLFWwindow* App::window;
ImGuiIO* App::imgui_io;

void App::glfw_error_callback(int error, const char* desc)
{
	std::string log = fmt::format("Glfw error %d: %s", error, desc);
	PLOGE << log;
}

void App::init()
{
#if DEBUG_MODE
	static plog::ColorConsoleAppender<plog::TxtFormatter> console_appender;
	plog::init(plog::verbose, &console_appender);
#else
	plog::init(plog::verbose, "log.txt");
#endif
}

int App::init_app()
{
	PLOGI << "Application starting...";

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
		return RES_FAIL;

	CodeNect::App::init_window();
	CodeNect::App::init_imgui();

	return RES_SUCCESS;
}

void App::init_window()
{
	App::window = glfwCreateWindow(CodeNect::Config::win_width, CodeNect::Config::win_height,
		CodeNect::Config::app_title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(App::window);
	glfwSwapInterval(CodeNect::Config::vsync);
}

void App::init_imgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	App::imgui_io = &ImGui::GetIO(); (void)App::imgui_io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(App::window, true);
	ImGui_ImplOpenGL2_Init();
}

void App::render_start()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void App::render_end()
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

void App::shutdown()
{
	PLOGI << "Application closing...";

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(App::window);
	glfwTerminate();
}
}
