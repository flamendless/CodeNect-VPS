#include "ui/tutorial.hpp"

#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/commands.hpp"
#include "core/config.hpp"
#include "core/font.hpp"
#include "core/project.hpp"
#include "core/utils.hpp"
#include "core/markdown.hpp"
#include "node/nodes.hpp"
#include "node/node_print.hpp"
#include "modules/transpiler.hpp"
#include "ui/alert.hpp"
#include "ui/sidebar.hpp"
#include "ui/command_palette.hpp"

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
int Tutorial::current_step = -1;

std::string md_open_sidebar =
#include "markdown/tutorial_open_sidebar.md"
;
std::string md_open_cmd_palette =
#include "markdown/tutorial_open_cmd_palette.md"
;
std::string md_create_new_proj =
#include "markdown/tutorial_new_proj.md"
;
std::string md_navigation =
#include "markdown/tutorial_navigation.md"
;
std::string md_create_vars =
#include "markdown/tutorial_create_vars.md"
;
std::string md_create_op =
#include "markdown/tutorial_create_op.md"
;
std::string md_create_print =
#include "markdown/tutorial_create_print.md"
;
std::string md_transpile =
#include "markdown/tutorial_transpile.md"
;
std::string md_run =
#include "markdown/tutorial_run.md"
;

std::vector<TutorialInfo> Tutorial::v_steps = {
	{
		"Open Sidebar", false, std::move(md_open_sidebar),
		[](){ return Sidebar::is_open; }
	},
	{
		"Open Command Palette", false, std::move(md_open_cmd_palette),
		[](){ return CommandPalette::is_open; }
	},
	{
		"Create New Project", false, std::move(md_create_new_proj),
		[](){ return Project::has_open_proj; }
	},
	{
		"Navigation", false, std::move(md_navigation),
		[](){ return true; }
	},

	{
		"Create Node Variables", false, std::move(md_create_vars),
		[](){
			int n = std::count_if(Nodes::v_nodes.begin(), Nodes::v_nodes.end(),
				[](Node* &node){ return node->m_kind == +NODE_KIND::VARIABLE; });
			return n >= 3;
		}
	},

	{
		"Create Node Operation", false, std::move(md_create_op),
		[](){
			for (Node* &node : Nodes::v_nodes)
			{
				if (node->m_kind == +NODE_KIND::OPERATION)
					return true;
			}
			return false;
		}
	},

	{
		"Create Node Print", false, std::move(md_create_print),
		[](){
			for (Node* &node : Nodes::v_nodes)
			{
				NodePrint* node_print = dynamic_cast<NodePrint*>(node);
				if (node_print)
					return true;
			}
			return false;
		}
	},

	{
		"Transpiling Visual Code", false, std::move(md_transpile),
		[](){ return Transpiler::has_compiled; }
	},
	{
		"Running Visual Code", false, std::move(md_run),
		[](){ return Transpiler::has_ran; }
	},
};

int Tutorial::init(void)
{
	const int x = Config::win_width;
	const int y = (float)Config::win_height/2;

	Tutorial::pos = ImVec2(x, y);
	Tutorial::size = ImVec2(Config::win_width * 0.45, Config::win_height - 16);

	if (Config::first_time && !Config::tutorial_done)
	{
		Alert::open(ALERT_TYPE::SUCCESS, "Hi, this is the first time you have\nopened this software, would you like to start the tutorial?");
		Alert::has_cb = true;
		Alert::fn_custom_draw = []()
		{
			if (ImGui::Button(ICON_FA_CHECK " Yes, begin the tutorial"))
			{
				Tutorial::is_open = true;
				Alert::close();
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_TIMES " No, I already know how to use it"))
			{
				Tutorial::is_open = false;
				Config::first_time = false;
				Config::tutorial_done = true;
				Config::ini.SetValue("general", "first_time", "false");
				Config::ini.SetValue("general", "tutorial_done", "true");
				Config::save_user_config();
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
		static int finished = 0;

		if (Tutorial::current_step == -1)
		{
			Font::use_font(FONT_SIZE::LARGE);
			Utils::center_text(ICON_FA_AWARD " WELCOME!", true);
			Utils::center_text("This tutorial will teach you the", true);
			Utils::center_text("basics of visual programming", true);
			Utils::center_text("and how to use CodeNect", true);
			Font::unuse_font();
			if (ImGui::Button("Proceed to the tutorial"))
				Tutorial::current_step = 0;
		}
		else
		{
			int step = 0;
			for (TutorialInfo& info : Tutorial::v_steps)
			{
				if (info.m_is_done)
				{
					ImGui::TextColored(ImVec4(0, 1, 0, 1), ICON_FA_CHECK);
					ImGui::SameLine();
				}
				if (step == Tutorial::current_step)
					ImGui::SetNextItemOpen(true);
				else if (!info.m_is_done)
				{
					++step;
					continue;
				}

				ImGui::PushID(step);
				if (ImGui::TreeNode(info.m_title))
				{
					if (step == Tutorial::current_step)
					{
						Markdown::draw(info.m_content);
						if (info.m_cond())
							info.m_is_done = true;
						if (info.m_is_done && ImGui::Button("Proceed to next tutorial"))
						{
							++Tutorial::current_step;
							++finished;
						}
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
				++step;
			}
		}

		//if all is done
		if (finished >= (int)Tutorial::v_steps.size())
		{
			ImGui::Separator();
			Font::use_font(FONT_SIZE::LARGE);
			Utils::center_text(ICON_FA_AWARD " CONGRATULATIONS!", true);
			Utils::center_text("You have finished the tutorial.", true);
			Utils::center_text("You may now proceed to making visual codes!", true);
			Font::unuse_font();
			if (ImGui::Button("Close the tutorial"))
			{
				Config::first_time = false;
				Config::tutorial_done = true;
				Config::ini.SetValue("general", "first_time", "false");
				Config::ini.SetValue("general", "tutorial_done", "true");
				Config::save_user_config();
				Tutorial::is_open = false;
			}
		}
		ImGui::End();
	}

	ImGui::PopStyleVar(1);
}
}
