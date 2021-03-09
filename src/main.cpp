#include "core/config.hpp"
#include "core/app.hpp"
#include "core/font.hpp"
#include "modules/filesystem.hpp"
#include "ui/sidebar.hpp"
#include "ui/sidebar_indicator.hpp"
#include "ui/sidebar_handler.hpp"
#include "ui/node_interface.hpp"
#include "ui/alert.hpp"
#include "core/project.hpp"
#include "core/defines.hpp"
#include "ui/command_palette.hpp"

int main(int argv, char** args)
{
	CodeNect::App app;
	app.init();

	if (CodeNect::Config::init() != RES_SUCCESS) return -1;

	if (app.init_app() != RES_SUCCESS) return -1;

	CodeNect::Font::init();

	//Command Palette
	if (CodeNect::CommandPalette::init() != RES_SUCCESS) return -1;

	CodeNect::Project::init();

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

	ImGuiIO* imgui_io = &ImGui::GetIO();

	while(!glfwWindowShouldClose(CodeNect::App::window))
	{
		float dt = imgui_io->DeltaTime;

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
		CodeNect::Project::draw();
		CodeNect::CommandPalette::draw();
		CodeNect::Alert::draw();

		app.render_end();
	}

	app.shutdown();

	return 0;
}
