#include "ui/command_palette.hpp"

#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "modules/input.hpp"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"
#include "core/config.hpp"

namespace CodeNect
{
bool CommandPalette::is_open = false;
const char* CommandPalette::title = ICON_FA_TERMINAL " Command Palette";
const char* CommandPalette::str_close = "press <ESC> to close";
ImVec2 CommandPalette::size;
ImGuiWindowFlags CommandPalette::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse;
ImGuiTextFilter CommandPalette::filter;
int CommandPalette::cur_pos = 0;
int CommandPalette::cur_cmd = -1;

int CommandPalette::init(void)
{
	Input::register_keypress(CommandPalette::keypress);

	return RES_SUCCESS;
}

void CommandPalette::open(void)
{
	CommandPalette::size = Config::CommandPalette_c::size;
	CommandPalette::is_open = true;

	PLOGV << "Command Palette launched";
}

void CommandPalette::close(void)
{
	CommandPalette::cur_pos = 0;
	CommandPalette::cur_cmd = -1;
	CommandPalette::is_open = false;

	PLOGV << "Command Palette closed";
}

void CommandPalette::keypress(GLFWwindow* window, int key, int scancode, int mods)
{
	if (key == GLFW_KEY_P && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
	{
		CommandPalette::open();
	}
	else if (key == GLFW_KEY_ESCAPE && CommandPalette::is_open)
	{
		CommandPalette::close();
	}
	else if (key == GLFW_KEY_UP && CommandPalette::is_open)
	{
		CommandPalette::cur_pos--;

		if (CommandPalette::cur_pos < 0)
			CommandPalette::cur_pos = CommandPalette::filter.CountGrep;
	}
	else if (key == GLFW_KEY_DOWN && CommandPalette::is_open)
	{
		CommandPalette::cur_pos++;

		if (CommandPalette::cur_pos > CommandPalette::filter.CountGrep)
			CommandPalette::cur_pos = 0;
	}
	else if (key == GLFW_KEY_ENTER && CommandPalette::is_open)
	{
		if (CommandPalette::cur_cmd == -1)
			return;

		const Command* cmd = Commands::v_cmd[CommandPalette::cur_cmd];
		cmd->run();

		CommandPalette::filter.Clear();

		if (cmd->m_close_command_palette)
			CommandPalette::close();

		PLOGV << "Command launched: " << cmd->m_title;
	}
}

void CommandPalette::draw(void)
{
	if (!CommandPalette::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(CommandPalette::size);

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("CommandPalette", &CommandPalette::is_open, CommandPalette::flags))
	{
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(CommandPalette::title, true);
		ImGui::Separator();

		float font_size = (Font::get_font_size(FONT_SIZE::LARGE) * 2) + 8;
		static ImVec4 color(1, 1, 1, 0.6f);
		ImGui::SetNextItemWidth(CommandPalette::size.x - font_size);
		CommandPalette::filter.Draw(ICON_FA_SEARCH);
		ImGui::SetKeyboardFocusHere(-1);
		int cur = 0;

		for (int i = 0; i < Commands::v_cmd.size(); i++)
		{
			const Command* cmd = Commands::v_cmd[i];
			if (CommandPalette::filter.PassFilter(cmd->m_title.c_str()))
			{
				if (CommandPalette::filter.CountGrep != 0)
				{
					if (cur_pos == cur)
					{
						ImGui::Text(ICON_FA_ARROW_RIGHT "    %s %s", cmd->m_icon, cmd->m_title.c_str());
						CommandPalette::cur_cmd = i;
					}
					else
						ImGui::TextColored(color, "%s %s", cmd->m_icon, cmd->m_title.c_str());

					ImGui::SameLine();
					ImGui::TextColored(color, "(%s)", cmd->m_desc.c_str());
					cur++;
				}
			}
		}

		ImGui::Separator();
		Utils::center_text(CommandPalette::str_close);
		ImGui::TextColored(color, "%s", CommandPalette::str_close);

		Font::unuse_font();
		ImGui::End();
	}
}
}
