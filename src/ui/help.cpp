#include "ui/help.hpp"

#include "IconsFontAwesome5Brands.h"
#include "plog/Log.h"
#include "core/commands.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
ImGuiWindowFlags Help::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Help::is_open = false;
const char* Help::title = ICON_FA_QUESTION_CIRCLE " HELP";

Help::pair_t Help::v_items
{
	std::make_pair<const char*, const char*>(ICON_FA_ENVELOPE " E-Mail:", "mailto:flamendless8@gmail.com"),
	std::make_pair<const char*, const char*>(ICON_FA_GITHUB " Github:", "https://github.com/flamendless/CodeNect-VPS"),
};

using help_t = std::tuple<const char*, const char*, const char*>;

Help::tuple_t Help::v_key_items
{
	help_t(ICON_FA_MOUSE " LMB", "select node", "left mouse button inside a node"),
	help_t(ICON_FA_MOUSE " RMB in canvas", "open node interface menu", "right mouse button click in the canvas"),
	help_t(ICON_FA_MOUSE " RMB in Node", "open nodes menu", "right mouse button click in the node"),
	help_t(ICON_FA_MOUSE " 2xLMB", "delete hovered connection", "hovering on a connection and then double left clicking will remove the connection"),
	help_t(ICON_FA_MOUSE " LMB + Drag", "move node", "hold left mouse button then drag"),
	help_t(ICON_FA_MOUSE " MMB + Drag", "pan view", "hold middle mouse button then drag"),
	help_t(ICON_FA_MOUSE " Ctrl + Scroll Wheel", "zoom in/out", "hold 'ctrl' key in then scroll middle mouse button/wheel"),
	help_t(ICON_FA_BACKSPACE " Delete Key", "delete selected node", "'del' key in keyboard"),
	help_t(ICON_FA_KEYBOARD " Enter/Return Key", "close alert popup window", "'Enter'/'Return' key in keyboard"),
	help_t(ICON_FA_KEYBOARD " Escape Key", "close inspector window", "'Esc' key in keyboard"),
};

void Help::register_commands(void)
{
	Command* cmd = new Command("Help", "open help window", ICON_FA_QUESTION_CIRCLE);
	cmd->set_fn(Help::open);
	cmd->m_close_command_palette = true;

	Command* cmd_alt = new Command("Keyboard Controls", "open keyboard controls window", ICON_FA_KEYBOARD);
	cmd_alt->set_fn(Help::open);
	cmd_alt->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
	Commands::register_cmd(*cmd_alt);
}

void Help::open(void)
{
	Help::is_open = true;
	PLOGD << "Opened Help window";
}

void Help::draw()
{
	if (!Help::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("Help", &Help::is_open, Help::flags))
	{
		Utils::center_text(Help::title, true);
		ImGui::Separator();

		Help::draw_commands();
		ImGui::Separator();

		Help::draw_support();
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
			Help::is_open = false;

		ImGui::End();
	}
}

void Help::draw_commands(void)
{
	Utils::center_text(ICON_FA_KEYBOARD " Commands", true);

	if (ImGui::BeginTable("TableCommands", 3))
	{
		for (const std::tuple<const char*, const char*, const char*> &item : Help::v_key_items)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", std::get<0>(item));

			ImGui::TableNextColumn();
			ImGui::Text("%s", std::get<1>(item));

			ImGui::TableNextColumn();
			Utils::help_marker(std::get<2>(item));
		}

		ImGui::EndTable();
	}
}

void Help::draw_support(void)
{
	Utils::center_text(ICON_FA_ADDRESS_BOOK " Contacts", true);

	if (ImGui::BeginTable("TableSupport", 3))
	{
		for (const std::pair<const char*, const char*> &item : Help::v_items)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", item.first);

			ImGui::TableNextColumn();
			ImGui::Text("%s", item.second);

			ImGui::TableNextColumn();
			ImGui::PushID(item.second);

			if (ImGui::SmallButton(ICON_FA_EXTERNAL_LINK_ALT))
			{
				Utils::open_url(item.second);
				Alert::open(ALERT_TYPE::SUCCESS, "Link opened in browser");
			}

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Open link in browser");

			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
}
