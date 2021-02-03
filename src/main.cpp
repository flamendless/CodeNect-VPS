#include "app.h"
#include "filesystem.h"
#include "sidebar.h"

int main(int argv, char** args)
{
	int app_status = App::init();

	if (app_status != 0) return -1;

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
		App::update(is_running);
		App::render_start();

#if DEBUG_MODE
		if (is_imgui_demo) ImGui::ShowDemoWindow(&is_imgui_demo);
#endif

		//draw other here
		Sidebar::draw();

		App::render_end();
	}

	App::shutdown();

	return 0;
}
