#include "ui/zoom.hpp"

#include "IconsFontAwesome5.h"
#include "core/commands.hpp"
#include "core/config.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
ImGuiWindowFlags Zoom::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Zoom::is_open = false;
ImVec2 Zoom::pos;
ImVec2 Zoom::size;
float Zoom::zoom_factor = 1.0f;

int Zoom::init(void)
{
	const int x = Config::win_width - 8;
	const int y = (float)Config::win_height - 8;

	Zoom::pos = ImVec2(x, y);
	Zoom::size = ImVec2(64, 32);

	return RES_SUCCESS;
}

void Zoom::register_commands(void)
{
	Command* cmd_open = new Command("Show zoom panel", "open zoom window", ICON_FA_SEARCH_PLUS);
	cmd_open->set_fn(Zoom::open);
	cmd_open->m_close_command_palette = true;

	Command* cmd_close = new Command("Hide zoom panel", "close zoom window", ICON_FA_SEARCH_PLUS);
	cmd_close->set_fn(Zoom::close);
	cmd_close->m_close_command_palette = true;

	Commands::register_cmd(*cmd_open);
	Commands::register_cmd(*cmd_close);
}

void Zoom::open(void)
{
	Zoom::is_open = true;
}

void Zoom::close(void)
{
	Zoom::is_open = false;
}

void Zoom::draw(void)
{
	if (!Zoom::is_open)
		return;

	ImGui::SetNextWindowPos(Zoom::pos, ImGuiCond_Always, ImVec2(1, 1));
	ImGui::SetNextWindowSize(Zoom::size);

	if (ImGui::Begin("Zoom", &Zoom::is_open, Zoom::flags))
	{
		ImGui::Text(ICON_FA_SEARCH_PLUS " %d%%", (int)(Zoom::zoom_factor * 100));
		ImGui::End();
	}
}
}
