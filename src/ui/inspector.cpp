#include "ui/inspector.hpp"

#include <imgui_internal.h>
#include "fmt/format.h"
#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"
#include "core/commands.hpp"
#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/nodes.hpp"
#include "node/node_renderer.hpp"
#include "ui/node_interface.hpp"
#include "ui/create_node.hpp"
#include "modules/input.hpp"
#include "ui/docs.hpp"

namespace CodeNect
{
ImGuiWindowFlags Inspector::flags =
	ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_AlwaysAutoResize;
bool Inspector::is_open = false;
ImVec2 Inspector::pos;
ImVec2 Inspector::size;

int Inspector::init(void)
{
	const int x = Config::win_width;
	const int y = (float)Config::win_height/2;

	Inspector::pos = ImVec2(x, y);
	Inspector::size = ImVec2(Config::win_width * 0.30, Config::win_height - 16);
	Input::register_key_event(Inspector::keypress);

	return RES_SUCCESS;
}

void Inspector::register_commands(void)
{
	Command* cmd = new Command("Inspector", "open/close inspector window", ICON_FA_SEARCH);
	cmd->set_fn(Inspector::toggle);
	cmd->m_close_command_palette = true;
	Commands::register_cmd(*cmd);

	Command* cmd_find = new Command("Search Node", "search node through the Inspector", ICON_FA_SEARCH);
	cmd_find->set_fn([](){ Inspector::is_open = true; });
	cmd_find->m_close_command_palette = true;
	Commands::register_cmd(*cmd_find);
}

void Inspector::toggle(void)
{
	Inspector::is_open = !Inspector::is_open;
}

bool Inspector::keypress(int key, int, int mods)
{
	if (key == GLFW_KEY_I && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
		Inspector::toggle();

	return false;
}

void Inspector::draw(void)
{
	if (!Inspector::is_open)
		return;

	ImGui::SetNextWindowPos(Inspector::pos, ImGuiCond_Always, ImVec2(1, 0.5));
	ImGui::SetNextWindowSize(Inspector::size);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	if (ImGui::Begin("Inspector", &Inspector::is_open, Inspector::flags))
	{
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(ICON_FA_SEARCH " INSPECTOR", true);
		Font::unuse_font();
		Utils::center_text("press <ESC> to close", true);
		ImGui::Separator();

		Inspector::draw_all();
		Inspector::draw_variables();
		Inspector::draw_ds();
		ImGui::Separator();

		if (ImGui::CollapsingHeader(ICON_FA_SORT " ARRANGE NODES"))
		{
			if (ImGui::SmallButton("What is this?##doc_arrange"))
				Docs::open_doc_id(DOC_ID::ABOUT_ARRANGE);
			for (unsigned long int i = 0; i < Nodes::v_nodes.size(); i++)
			{
				int index = i;
				ImGui::PushID(i);
				if (ImGui::Button(ICON_FA_SORT_UP))
				{
					index = (i == 0) ? (Nodes::v_nodes.size() - 1) : (index - 1);
					Node* temp = Nodes::v_nodes[i];
					Node* upper = Nodes::v_nodes[index];
					Nodes::v_nodes[i] = upper;
					Nodes::v_nodes[index] = temp;
				}

				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_SORT_DOWN))
				{
					index = (i == Nodes::v_nodes.size() - 1) ? 0 : (index + 1);
					Node* temp = Nodes::v_nodes[i];
					Node* lower = Nodes::v_nodes[index];
					Nodes::v_nodes[i] = lower;
					Nodes::v_nodes[index] = temp;
				}
				ImGui::PopID();

				ImGui::SameLine();
				ImGui::Text("%s", Nodes::v_nodes[i]->m_name);
			}
		}

		ImGui::End();
	}

	ImGui::PopStyleVar(1);

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		Inspector::is_open = false;
}

void Inspector::draw_all(void)
{
	int i = 1;
	if (ImGui::TreeNode("All Nodes"))
	{
		for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
			it != Nodes::v_nodes.end();
			it++)
		{
			Node* node = *it;

			ImGui::PushID(node);
			std::string str = fmt::format("{}. {}", i, node->m_name);

			Inspector::draw_buttons(node);
			ImGui::SameLine();

			if (ImGui::TreeNode(str.c_str()))
			{
				if (ImGui::BeginTable("Info", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Name:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Kind:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");
					ImGui::TableNextColumn();
					ImGui::Text("%s", node->m_name);
					ImGui::Text("%s", node->m_kind._to_string());
					ImGui::Text("%s", node->m_desc);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}
		ImGui::TreePop();
	}
}

void Inspector::draw_variables(void)
{
	int i = 1;
	if (ImGui::TreeNode("Variables"))
	{
		for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
			it != Nodes::v_nodes.end();
			it++)
		{
			NodeVariable* node_var = dynamic_cast<NodeVariable*>(*it);

			if (!node_var)
				continue;

			ImGui::PushID(node_var);
			std::string str = fmt::format("{}. {}", i, node_var->m_name);

			Inspector::draw_buttons(node_var);
			ImGui::SameLine();

			if (ImGui::TreeNode(str.c_str()))
			{
				if (ImGui::BeginTable("Info", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Name:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Type:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Value:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");
					ImGui::TableNextColumn();
					ImGui::Text("%s", node_var->m_name);
					ImGui::Text("%s", node_var->m_value_orig.m_slot._to_string());
					NodeRenderer::draw_node_val(&node_var->m_value);
					ImGui::Text("%s", node_var->m_desc);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}
		ImGui::TreePop();
	}
}

void Inspector::draw_ds(void)
{
	int i = 1;
	if (ImGui::TreeNode("Data Structures"))
	{
		for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
			it != Nodes::v_nodes.end();
			it++)
		{
			NodeArray* node_array = dynamic_cast<NodeArray*>(*it);

			if (!node_array)
				continue;

			ImGui::PushID(node_array);
			std::string str = fmt::format("{}. {}", i, node_array->m_name);

			Inspector::draw_buttons(node_array);
			ImGui::SameLine();

			if (ImGui::TreeNode(str.c_str()))
			{
				if (ImGui::BeginTable("Info", 2, ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Name:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Type:");
					ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");
					ImGui::TableNextColumn();
					ImGui::Text("%s", node_array->m_name);
					ImGui::Text("%s", node_array->m_slot._to_string());
					ImGui::Text("%s", node_array->m_desc);
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}
		ImGui::TreePop();
	}
}

void Inspector::draw_buttons(Node* node)
{
	if (ImGui::SmallButton(ICON_FA_SEARCH))
		NodeInterface::jump_to_pos(node);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Jump the camera's position to the node's position");

	ImGui::SameLine();

	if (ImGui::SmallButton(ICON_FA_EDIT))
		CreateNode::edit(node);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Edit node");
}
}
