#include "config.hpp"
#include "app.hpp"
#include "filesystem.hpp"
#include "sidebar.hpp"
#include "sidebar_indicator.hpp"
#include "sidebar_handler.hpp"
#include "defines.hpp"

int main(int argv, char** args)
{
	CodeNect::App::init();

	const int config_status = CodeNect::Config::init();
	if (config_status != RES_SUCCESS) return -1;

	const int app_status = CodeNect::App::init_app();
	if (app_status != RES_SUCCESS) return -1;

	// std::string project_filepath;
	// bool opened_project = Filesystem::open_project_file(project_filepath);
    //
	// if (opened_project)
	// 	fmt::print(stdout, project_filepath);
	// else
	// 	fmt::print(stdout, "no");

	//initialize modules/sources

	//Sidebar
	const int sidebar_status = CodeNect::Sidebar::init();
	if (sidebar_status != RES_SUCCESS) return -1;

	const int sidebar_indicator_status = CodeNect::SidebarIndicator::init();
	if (sidebar_indicator_status != RES_SUCCESS) return -1;

	CodeNect::SidebarHandler::init();

#if DEBUG_MODE
	bool is_imgui_demo = true;
#endif

	while(!glfwWindowShouldClose(CodeNect::App::window))
	{
		float dt = CodeNect::App::imgui_io->DeltaTime;

		glfwPollEvents();

		//update
		CodeNect::SidebarIndicator::update(dt);
		CodeNect::Sidebar::update(dt);
		CodeNect::SidebarHandler::update(dt);

		CodeNect::App::render_start();

#if DEBUG_MODE
		if (is_imgui_demo) ImGui::ShowDemoWindow(&is_imgui_demo);
#endif

		//draw other here
		CodeNect::Sidebar::draw();
		CodeNect::SidebarIndicator::draw();

		CodeNect::App::render_end();
	}

	CodeNect::App::shutdown();

	return 0;
}
