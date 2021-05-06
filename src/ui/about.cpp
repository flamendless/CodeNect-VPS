#include "ui/about.hpp"

#include "imgui_internal.h"
#include "plog/Log.h"
#include "core/utils.hpp"
#include "core/config.hpp"
#include "core/commands.hpp"
#include "core/font.hpp"

namespace CodeNect
{
ImGuiWindowFlags About::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool About::is_open = false;
const char* About::title = ICON_FA_INFO_CIRCLE " ABOUT";

void About::register_commands(void)
{
	Command* cmd = new Command("About", "open about window", ICON_FA_INFO_CIRCLE);
	cmd->set_fn(About::open);
	cmd->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
}

void About::open(void)
{
	About::is_open = true;
	PLOGD << "Opened About window";
}

void About::draw()
{
	if (!About::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("About", &About::is_open, About::flags))
	{
		Utils::center_text(About::title, true);
		ImGui::Separator();

		Utils::center_text(ICON_FA_PROJECT_DIAGRAM " CodeNect: Visual Programming", true);
		Utils::center_text("Software for Learning", true);
		Utils::center_text("Fundamentals of Programming", true);
		Utils::center_text(Config::version, true);
		ImGui::Separator();

		ImGui::Text("%s Thesis by:", ICON_FA_BOOK);
		ImGui::Indent();
			ImGui::Text("%s Brandon Blanker Lim-it", ICON_FA_USER_COG);
			ImGui::Text("%s Jaykel O. Punay", ICON_FA_USER_NINJA);
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("%s Cavite State University", ICON_FA_SCHOOL);
		ImGui::Text("%s BSIT Batch 2017-2021", ICON_FA_USERS);
		ImGui::Separator();

		ImGui::Text("%s Special Thanks to:", ICON_FA_STAR);
		ImGui::Indent();
			ImGui::Text("%s James Angelo V. Aves (Adviser)", ICON_FA_USER_CHECK);
			ImGui::Text("%s Annabelle Almares (Technical Critic)", ICON_FA_USER_EDIT);
		ImGui::Unindent();
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
			About::is_open = false;

		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		About::is_open = false;
}
}
