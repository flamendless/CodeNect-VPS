#include "app.h"

namespace App
{
int init()
{
	fmt::print(stdout, "Application starting...\n");

	int flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;

	if (SDL_Init(flags) != 0)
	{
		fmt::printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	App::init_window();
	App::init_imgui();

	return 0;
}

void init_window()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	App::window = SDL_CreateWindow(Config::app_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::win_width, Config::win_height, window_flags);

	App::gl_context = SDL_GL_CreateContext(App::window);
	SDL_GL_MakeCurrent(App::window, App::gl_context);
	SDL_GL_SetSwapInterval(Config::vsync);
}

void init_imgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	App::imgui_io = &ImGui::GetIO(); (void)App::imgui_io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App::window, App::gl_context);
	ImGui_ImplOpenGL2_Init();
}

void update(bool& is_running)
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT) is_running = false;
		if (event.type == SDL_WINDOWEVENT &&
			event.window.event == SDL_WINDOWEVENT_CLOSE &&
			event.window.windowID == SDL_GetWindowID(App::window))
			is_running = false;
	}
}

void render_start()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App::window);
	ImGui::NewFrame();
}

void render_end()
{
	ImVec4& clear_color = Config::clear_color;

	ImGui::Render();
	glViewport(0, 0, (int)App::imgui_io->DisplaySize.x, (int)App::imgui_io->DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App::window);
}

void shutdown()
{
	fmt::print(stderr, "Application exiting...\n");

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(App::gl_context);
	SDL_DestroyWindow(App::window);
	SDL_Quit();
}
}
