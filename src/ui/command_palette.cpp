#include "ui/command_palette.hpp"

#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "modules/input.hpp"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"

namespace CodeNect
{
bool CommandPalette::is_open = false;
const char* CommandPalette::title = ICON_FA_TERMINAL " Command Palette";
ImGuiWindowFlags CommandPalette::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;

int CommandPalette::init(void)
{
	Input::register_keypress(CommandPalette::keypress);

	return RES_SUCCESS;
}

void CommandPalette::keypress(GLFWwindow* window, int key, int scancode, int mods)
{
	if (key == GLFW_KEY_P && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
	{
		CommandPalette::is_open = true;

		PLOGV << "Command Palette launched";

		// for (const Command* cmd : Commands::v_cmd)
		// {
		// 	PLOGD << "Command: " << cmd->title;
		// }
	}
	else if (key == GLFW_KEY_ESCAPE && CommandPalette::is_open)
	{
		CommandPalette::is_open = false;

		PLOGV << "Command Palette closed";
	}
}

void CommandPalette::draw(void)
{
	if (!CommandPalette::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("CommandPalette", &CommandPalette::is_open, CommandPalette::flags))
	{
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(CommandPalette::title);
		ImGui::Text("%s", CommandPalette::title);

		ImGui::TextColored(ImVec4(1, 1, 1, 0.75f), "press <ESC> to close");

		Font::unuse_font();
		ImGui::End();
	}
}
}
