#include "ui/terminal.hpp"

#include <imgui_internal.h>
#include "IconsFontAwesome5.h"
#include "core/config.hpp"
#include "core/commands.hpp"
#include "core/font.hpp"
#include "core/utils.hpp"
#include "modules/transpiler.hpp"

namespace CodeNect
{
ImGuiWindowFlags Terminal::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Terminal::is_open = false;
ImVec2 Terminal::pos;
ImVec2 Terminal::size;

int Terminal::init(void)
{
	const int x = (float)Config::win_width/2;
	const int y = (float)Config::win_height/2;

	Terminal::pos = ImVec2(x, y);
	Terminal::size = ImVec2(640, 480);

	return RES_SUCCESS;
}

void Terminal::register_commands(void)
{
	Command* cmd = new Command("Terminal", "open/close the terminal", ICON_FA_CODE);
	cmd->set_fn(Terminal::toggle);
	cmd->m_close_command_palette = true;
	Commands::register_cmd(*cmd);
}

void Terminal::toggle(void)
{
	Terminal::is_open = !Terminal::is_open;
}

void Terminal::draw(void)
{
	if (!Terminal::is_open)
		return;

	ImGui::SetNextWindowPos(Terminal::pos, ImGuiCond_Once, ImVec2(0.5, 0.5));
	ImGui::SetNextWindowSize(Terminal::size);

	if (ImGui::Begin("Terminal", &Terminal::is_open, Terminal::flags))
	{
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(ICON_FA_CODE " Terminal", true);
		Font::unuse_font();
		Utils::center_text("press <ESC> to close", true);
		ImGui::Separator();

		if (ImGui::SmallButton("Compile"))
			Transpiler::compile();
		ImGui::SameLine();
		if (ImGui::SmallButton("Run"))
			Transpiler::run();
		ImGui::SameLine();
		if (ImGui::SmallButton("Clear"))
			Transpiler::clear();

		ImGui::Separator();

		for (const std::string& str : Transpiler::v_output)
			ImGui::Text("%s", str.c_str());

		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		Terminal::is_open = false;
}
}
