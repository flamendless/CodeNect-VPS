#include "app.hpp"

namespace CodeNect
{
void glfw_error_callback(int error, const char* desc)
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

	App::init_window();
	App::init_imgui();

	return RES_SUCCESS;
}

void App::init_window()
{
	m_window = glfwCreateWindow(CodeNect::Config::win_width, CodeNect::Config::win_height,
		CodeNect::Config::app_title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(CodeNect::Config::vsync);
}

void App::init_imgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_imgui_io = &ImGui::GetIO(); (void)m_imgui_io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
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
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(m_window);
	glfwSwapBuffers(m_window);
}

void App::shutdown()
{
	PLOGI << "Application closing...";

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
}
