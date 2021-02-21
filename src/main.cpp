#include "config.hpp"
#include "app.hpp"
#include "font.hpp"
#include "filesystem.hpp"
#include "sidebar.hpp"
#include "sidebar_indicator.hpp"
#include "sidebar_handler.hpp"
#include "node_interface.hpp"
#include "defines.hpp"

int main(int argv, char** args)
{
	CodeNect::App app;
	app.init();

	if (CodeNect::Config::init() != RES_SUCCESS) return -1;

	if (app.init_app() != RES_SUCCESS) return -1;

	CodeNect::Font::init();

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

	if (sidebar.init() != RES_SUCCESS) return -1;

	if (sidebar_indicator.init() != RES_SUCCESS) return -1;

	sidebar_handler.init(&sidebar, &sidebar_indicator);

	//Interfaces
	if (CodeNect::NodeInterface::init() != RES_SUCCESS) return -1;

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
		CodeNect::NodeInterface::draw();

		app.render_end();
	}

	app.shutdown();

	return 0;
}
