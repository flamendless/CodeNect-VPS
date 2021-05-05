#include <filesystem>
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
#include "ui/docs.hpp"
#include "ui/settings.hpp"
#include "ui/about.hpp"
#include "ui/help.hpp"
#include "ui/inspector.hpp"
#include "ui/terminal.hpp"
#include "ui/zoom.hpp"
#include "modules/transpiler.hpp"

int main(int argv, char** args)
{
	CodeNect::App app;
	app.init();

	if (CodeNect::Config::init() != RES_SUCCESS) return -1;

	if (app.init_app() != RES_SUCCESS) return -1;

	CodeNect::Font::init();

	//Command Palette
	if (CodeNect::CommandPalette::init() != RES_SUCCESS) return -1;
	if (CodeNect::Alert::init() != RES_SUCCESS) return -1;

	CodeNect::App::register_commands();
	CodeNect::Project::register_commands();
	CodeNect::Settings::register_commands();
	CodeNect::About::register_commands();
	CodeNect::Help::register_commands();

	if (CodeNect::Docs::init() != RES_SUCCESS) return -1;
	CodeNect::Docs::register_commands();

	//Inspector
	if (CodeNect::Inspector::init() != RES_SUCCESS) return -1;
	CodeNect::Inspector::register_commands();

	//Zoom
	if (CodeNect::Zoom::init() != RES_SUCCESS) return -1;
	CodeNect::Zoom::register_commands();

	//Sidebar
	CodeNect::Sidebar sidebar;
	CodeNect::SidebarIndicator sidebar_indicator;
	CodeNect::SidebarHandler sidebar_handler;

	if (sidebar.init() != RES_SUCCESS) return -1;
	if (sidebar_indicator.init() != RES_SUCCESS) return -1;
	sidebar_handler.init(&sidebar, &sidebar_indicator);

	//Interfaces
	if (CodeNect::NodeInterface::init() != RES_SUCCESS) return -1;

	//Transpiler
	if (CodeNect::Transpiler::init() != RES_SUCCESS) return -1;
	CodeNect::Transpiler::register_commands();

	//Terminal
	if (CodeNect::Terminal::init() != RES_SUCCESS) return -1;
	CodeNect::Terminal::register_commands();

#if DEBUG_MODE
	bool is_imgui_demo = true;
#endif

	ImGuiIO* imgui_io = &ImGui::GetIO();

	//test
#if DEBUG_MODE
	CodeNect::Project::open("test_arrays.cn");
#endif

	if (argv > 1)
	{
		const char* filepath = args[1];
		if (std::filesystem::exists(filepath))
			CodeNect::Project::open(filepath);
	}

	PLOGI << "Initialization is complete";

	while(!glfwWindowShouldClose(CodeNect::App::window))
	{
		float dt = imgui_io->DeltaTime;

		glfwPollEvents();

		//update
		sidebar_handler.update(dt);
		CodeNect::NodeInterface::update(dt);

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
		CodeNect::Inspector::draw();
		CodeNect::Zoom::draw();
		CodeNect::Terminal::draw();
		CodeNect::Docs::draw();

		app.render_end();
	}

	sidebar.shutdown();
	CodeNect::Commands::shutdown();
	CodeNect::Font::shutdown();
	CodeNect::Config::shutdown();
	CodeNect::Project::shutdown();
	CodeNect::Transpiler::shutdown();
	app.shutdown();

	return 0;
}
