#include "config.hpp"
#include "app.hpp"
#include "filesystem.hpp"
#include "sidebar.hpp"
#include "sidebar_indicator.hpp"
#include "sidebar_handler.hpp"
#include "defines.hpp"

int main(int argv, char** args)
{
	CodeNect::App app;
	app.init();

	const int config_status = CodeNect::Config::init();
	if (config_status != RES_SUCCESS) return -1;

	const int app_status = app.init_app();
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
	CodeNect::Sidebar sidebar;
	CodeNect::SidebarIndicator sidebar_indicator;
	CodeNect::SidebarHandler sidebar_handler;

	const int sidebar_status = sidebar.init();
	if (sidebar_status != RES_SUCCESS) return -1;

	const int sidebar_indicator_status = sidebar_indicator.init();
	if (sidebar_indicator_status != RES_SUCCESS) return -1;

	sidebar_handler.init(&sidebar, &sidebar_indicator);

#if DEBUG_MODE
	bool is_imgui_demo = true;
#endif

	while(!glfwWindowShouldClose(app.m_window))
	{
		float dt = app.m_imgui_io->DeltaTime;

		glfwPollEvents();

		//update
		sidebar_indicator.update(dt);
		sidebar.update(dt);
		sidebar_handler.update(dt);

		app.render_start();

#if DEBUG_MODE
		if (is_imgui_demo) ImGui::ShowDemoWindow(&is_imgui_demo);
#endif

		//draw other here
		sidebar.draw();
		sidebar_indicator.draw();

		app.render_end();
	}

	app.shutdown();

	return 0;
}
