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
unsigned int Tutorial::current_step = 0;

std::string md_open_sidebar =
#include "markdown/tutorial_open_sidebar.md"
;
std::string md_open_cmd_prompt =
#include "markdown/tutorial_open_cmd_prompt.md"
;
std::string md_create_new_proj =
#include "markdown/tutorial_new_proj.md"
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
std::string md_connect_nodes =
#include "markdown/tutorial_connect_nodes.md"
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
		"Open Command Palette", false, std::move(md_open_cmd_prompt),
		[](){ return CommandPalette::is_open; }
	},
	{
		"Create New Project", false, std::move(md_create_new_proj),
		[](){ return Project::has_open_proj; }
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
		"Connecting Nodes", false, std::move(md_connect_nodes),
		[](){ return Nodes::count_connections() >= 8; }
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
		unsigned int finished = 0;
		unsigned int step = 0;

		for (TutorialInfo& info : Tutorial::v_steps)
		{
			ImGui::PushID(info.m_title);
			if (step == Tutorial::current_step)
				ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode(info.m_title))
			{
				if (info.m_is_done)
					ImGui::Text(ICON_FA_CHECK);
				if (!info.m_is_done && step == Tutorial::current_step)
				{
					if (info.m_cond())
					{
						info.m_is_done = true;
						++finished;
					}
					Markdown::draw(info.m_content);
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			++step;
		}

		//if all is done
		if (finished == Tutorial::v_steps.size())
		{
			Font::use_font(FONT_SIZE::LARGE);
			Utils::center_text(ICON_FA_AWARD " CONGRATULATIONS!", true);
			Utils::center_text("You have finished the tutorial.", true);
			Utils::center_text("You may now proceed to making visual codes!", true);
			Utils::center_text(ICON_FA_AWARD " CONGRATULATIONS!", true);
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
