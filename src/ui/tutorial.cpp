#include "ui/tutorial.hpp"

#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "core/config.hpp"
#include "core/font.hpp"
#include "core/utils.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
ImGuiWindowFlags Tutorial::flags =
	ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_AlwaysAutoResize;
bool Tutorial::is_open = false;
ImVec2 Tutorial::pos;
ImVec2 Tutorial::size;
unsigned int Tutorial::current_step = 0;
std::map<const std::string, bool> Tutorial::m_steps = {
	{"Open Sidebar", false},
	{"Open Command Palette", false},
	{"Create New Project", false},
	{"Create Node Variables", false},
	{"Create Node Operation", false},
	{"Create Node Print", false},
	{"Connecting Nodes", false},
	{"Transpiling Visual Code", false},
	{"Running Visual Code", false},
};

int Tutorial::init(void)
{
	const int x = Config::win_width;
	const int y = (float)Config::win_height/2;

	Tutorial::pos = ImVec2(x, y);
	Tutorial::size = ImVec2(Config::win_width * 0.30, Config::win_height - 16);

	if (Config::first_time && !Config::tutorial_done)
	{
		Alert::open(ALERT_TYPE::SUCCESS, "Hi, this is the first time you have\nopened this software, would you like to start the tutorial?");
		Alert::has_cb = true;
		Alert::fn_custom_draw = []()
		{
			if (ImGui::Button(ICON_FA_CHECK " Yes, begin the tutorial"))
			{
				Tutorial::is_open = true;
				// Config::first_time = false;
				// Config::ini.SetValue("general", "first_time", "false");
				// Config::save_user_config();
				Alert::close();
			}

			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_TIMES " No, I already know how to use it"))
			{
				Tutorial::is_open = false;
				// Config::first_time = false;
				// Config::ini.SetValue("general", "first_time", "false");
				// Config::save_user_config();
				Alert::close();
			}
		};
	}

	return RES_SUCCESS;
}

void Tutorial::register_commands(void)
{
	Command* cmd = new Command("Tutorial", "start the tutorial wizard", ICON_FA_MAGIC);
	cmd->set_fn(Tutorial::toggle);
	cmd->m_close_command_palette = true;
	Commands::register_cmd(*cmd);
}

void Tutorial::toggle(void)
{
	Tutorial::is_open = !Tutorial::is_open;
}

void Tutorial::draw(void)
{
	if (!Tutorial::is_open)
		return;

	ImGui::SetNextWindowPos(Tutorial::pos, ImGuiCond_Always, ImVec2(1, 0.5));
	ImGui::SetNextWindowSize(Tutorial::size);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	if (ImGui::Begin("Tutorial", &Tutorial::is_open, Tutorial::flags))
	{
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(ICON_FA_MAGIC " TUTORIAL", true);
		Font::unuse_font();
		ImGui::Separator();
		unsigned int step = 0;

		for (std::pair<const std::string, bool>& e : Tutorial::m_steps)
		{
			const char* id = e.first.c_str();
			ImGui::PushID(id);
			if (step == Tutorial::current_step)
				ImGui::SetNextItemOpen(true);

			if (ImGui::TreeNode(id))
			{
				ImGui::TreePop();
			}
			ImGui::PopID();
			++step;
		}

		ImGui::End();
	}

	ImGui::PopStyleVar(1);
}
}
