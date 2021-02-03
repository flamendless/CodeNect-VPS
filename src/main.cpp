#include "config.hpp"
#include "app.hpp"
#include "filesystem.hpp"
#include "sidebar.hpp"
#include "defines.hpp"

int main(int argv, char** args)
{
	CodeNect::App::init();

	int config_status = CodeNect::Config::init();

	if (config_status != RES_SUCCESS) return -1;

	int app_status = CodeNect::App::init_app();

	if (app_status != RES_SUCCESS) return -1;

	// std::string project_filepath;
	// bool opened_project = Filesystem::open_project_file(project_filepath);
    //
	// if (opened_project)
	// 	fmt::print(stdout, project_filepath);
	// else
	// 	fmt::print(stdout, "no");

	//initialize modules/sources
	Sidebar::init();

	bool is_running = true;

#if DEBUG_MODE
	bool is_imgui_demo = true;
#endif

	while (is_running)
	{
		CodeNect::App::update(is_running);
		CodeNect::App::render_start();

#if DEBUG_MODE
		if (is_imgui_demo) ImGui::ShowDemoWindow(&is_imgui_demo);
#endif

		//draw other here
		Sidebar::draw();

		CodeNect::App::render_end();
	}

	CodeNect::App::shutdown();

	return 0;
}
