#include "app.h"

int main(int argv, char** args)
{
	int app_status = App::init();

	if (app_status != 0) return -1;

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

		App::render_end();
	}

	App::shutdown();

	return 0;
}
