#include "ui/assessments.hpp"

#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include "core/config.hpp"
#include "core/commands.hpp"
#include "core/font.hpp"
#include "core/utils.hpp"
#include "core/markdown.hpp"
#include "modules/input.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
ImGuiWindowFlags AssessmentsUI::flags =
	ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_AlwaysAutoResize;
bool AssessmentsUI::is_open = false;
bool AssessmentsUI::is_open_assessment = false;
ImVec2 AssessmentsUI::pos;
ImVec2 AssessmentsUI::size;

int AssessmentsUI::init(void)
{
	const int x = Config::win_width;
	const int y = (float)Config::win_height/2;
	AssessmentsUI::pos = ImVec2(x, y);
	AssessmentsUI::size = ImVec2(Config::win_width * 0.30, Config::win_height - 16);
	Input::register_key_event(AssessmentsUI::keypress);
	return RES_SUCCESS;
}

void AssessmentsUI::register_commands(void)
{
	Command* cmd = new Command("Assessments", "open/close assessments window", ICON_FA_CLIPBOARD_CHECK);
	cmd->set_fn(AssessmentsUI::toggle);
	cmd->m_close_command_palette = true;
	Commands::register_cmd(*cmd);
}

bool AssessmentsUI::keypress(int key, int, int mods)
{
	if (key == GLFW_KEY_A && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
		AssessmentsUI::toggle();
	return false;
}

void AssessmentsUI::toggle(void)
{
	AssessmentsUI::is_open = !AssessmentsUI::is_open;
}

void AssessmentsUI::draw(void)
{
	if (!AssessmentsUI::is_open)
		return;

	ImGui::SetNextWindowPos(AssessmentsUI::pos, ImGuiCond_Always, ImVec2(1, 0.5));
	ImGui::SetNextWindowSize(AssessmentsUI::size);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));

	if (ImGui::Begin("AssessmentsUI", &AssessmentsUI::is_open, AssessmentsUI::flags))
	{
		if (AssessmentsUI::is_open_assessment)
			AssessmentsUI::draw_assessment();
		else
		{
			Font::use_font(FONT_SIZE::LARGE);
			Utils::center_text(ICON_FA_CLIPBOARD_CHECK " ASSESSMENTS", true);
			Font::unuse_font();
			ImGui::Separator();
			for (Assessment& a : Assessments::v_list)
			{
				ImGui::PushID(a.title);
				if (ImGui::SmallButton(ICON_FA_BOOK_OPEN " open"))
				{
					AssessmentsUI::is_open = true;
					AssessmentsUI::is_open_assessment = true;
					Assessments::current_assessment = a;
				}
				ImGui::SameLine();
				ImGui::Text("%s", a.title);
				ImGui::PopID();
			}

			ImGui::Separator();
			if (ImGui::Button(ICON_FA_TIMES " Close Assessments"))
				AssessmentsUI::is_open = false;
		}
		ImGui::End();
	}
	ImGui::PopStyleVar(1);

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
	{
		if (AssessmentsUI::is_open_assessment)
			AssessmentsUI::is_open_assessment = false;
		else
			AssessmentsUI::is_open = false;
	}
}

void AssessmentsUI::draw_assessment(void)
{
	Utils::center_text(Assessments::current_assessment.title, true);
	ImGui::Separator();
	Markdown::draw(Assessments::current_assessment.str_content);
	ImGui::Separator();

	if (!Assessments::has_assessment)
	{
		if (ImGui::Button("Do this assessment"))
			Assessments::has_assessment = true;
	}
	else
	{
		if (ImGui::Button("Cancel this assessment"))
		{
			Alert::open(ALERT_TYPE::WARNING, "Are you sure you want to stop trying this assessment?");
			Alert::has_cb = true;
			Alert::fn_custom_draw = []()
			{
				if (ImGui::Button(ICON_FA_CHECK " Yes"))
				{
					Assessments::has_assessment = false;
					Alert::close();
				}
				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_TIMES " No"))
					Alert::close();
			};
		}
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_ARROW_LEFT " Back"))
	{
		AssessmentsUI::is_open_assessment = false;
		Alert::close();
	}
}
}
