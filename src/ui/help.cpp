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

Help::items_t Help::v_items
{
	std::make_pair<const char*, const char*>(ICON_FA_ENVELOPE " E-Mail:", "mailto:flamendless8@gmail.com"),
	std::make_pair<const char*, const char*>(ICON_FA_GITHUB " Github:", "https://github.com/flamendless/CodeNect-VPS"),
};

void Help::register_commands(void)
{
	Command* cmd = new Command("Help", "open help window", ICON_FA_QUESTION_CIRCLE);
	cmd->set_fn(Help::open);
	cmd->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
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
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(Help::title, true);
		ImGui::Separator();

		Help::draw_support();
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
			Help::is_open = false;

		Font::unuse_font();
		ImGui::End();
	}
}

void Help::draw_support(void)
{
	if (ImGui::BeginTable("TableSupport", 2))
	{
		for (const std::pair<const char*, const char*> &item : Help::v_items)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", item.first);
			ImGui::TableNextColumn();

			ImGui::Text("%s", item.second);
			ImGui::SameLine();

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
