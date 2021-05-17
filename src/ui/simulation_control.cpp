#include "ui/simulation_control.hpp"

#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "core/commands.hpp"
#include "modules/simulation.hpp"

namespace CodeNect
{
const char* SimulationControl::title = ICON_FA_PLAY_CIRCLE " Simulation Controls";
ImGuiWindowFlags SimulationControl::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoScrollWithMouse;

bool SimulationControl::is_open = false;
ImVec2 SimulationControl::pos;

int SimulationControl::init(void)
{
	const int x = Config::win_width - 8;
	const int y = (float)Config::win_height/2;
	SimulationControl::pos.x = x;
	SimulationControl::pos.y = y;

	return RES_SUCCESS;
}

void SimulationControl::register_commands(void)
{
	Command* cmd_open = new Command("Show/Hide Simulation Control", "show/hide simulation control window", ICON_FA_PLAY_CIRCLE);
	cmd_open->set_fn(SimulationControl::toggle);
	cmd_open->m_close_command_palette = true;
	Commands::register_cmd(*cmd_open);
}

void SimulationControl::toggle(void)
{
	SimulationControl::is_open = !SimulationControl::is_open;
}

void SimulationControl::draw(void)
{
	if (!SimulationControl::is_open)
		return;

	ImGui::SetNextWindowPos(SimulationControl::pos, ImGuiCond_FirstUseEver, ImVec2(1, 1));

	if (ImGui::Begin(SimulationControl::title, &SimulationControl::is_open, SimulationControl::flags))
	{
		if (ImGui::Button(ICON_FA_STEP_BACKWARD))
			Simulation::iterate(-1);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Step iteration by -1");
		ImGui::SameLine();

		static float max_timer = 1.0f;
		const char* icon = Simulation::is_playing ? ICON_FA_PAUSE : ICON_FA_PLAY;

		if (ImGui::Button(icon))
			Simulation::play(max_timer);
		if (ImGui::IsItemHovered())
		{
			if (Simulation::is_playing)
				ImGui::SetTooltip("Pause the playing of iteration");
			else
				ImGui::SetTooltip("Play the iteration automatically");
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_STOP_CIRCLE))
			Simulation::reset();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Reset iterator to initial value");
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_STEP_FORWARD))
			Simulation::iterate(1);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Step iteration by 1");

		ImGui::SliderFloat("Per second", &max_timer, 0.1f, 5.0f);
		Utils::help_marker("How many seconds before going to next iteration", true);

		ImGui::End();
	}
}
}
