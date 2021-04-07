#include "ui/help.hpp"

#include "IconsFontAwesome5Brands.h"
#include "plog/Log.h"
#include "core/commands.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"
#include "ui/alert.hpp"
#include "node/node_colors.hpp"

namespace CodeNect
{
ImGuiWindowFlags Help::flags =
	ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Help::is_open = false;
const char* Help::title = ICON_FA_QUESTION_CIRCLE " HELP";

using help_p = std::pair<const char*, const char*>;
using help_t = std::tuple<const char*, const char*, const char*>;
using help_tc = std::tuple<const char*, const char*, ImVec4>;

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

Help::tuple_colors_t Help::v_connection_colors_items
{
	help_tc("Green", "path processed because the result of node branch is 'true'", NodeColors::ConnectionColors::TRUE),
	help_tc("Red", "path NOT processed because the result of node branch is 'false'", NodeColors::ConnectionColors::FALSE),
	help_tc("Red", "value can't be added to array because it exceeds array's fixed size", NodeColors::ConnectionColors::FALSE),
	help_tc("Yellow", "path can only be determined during runtime", NodeColors::ConnectionColors::RUNTIME),
};

Help::tuple_colors_t Help::v_node_colors_items
{
	help_tc("Blue", "node is of kind 'Variable'/'Data Structure'", NodeColors::Lookup::BLUE),
	help_tc("Yellow", "node is of kind 'Operation'", NodeColors::Lookup::YELLOW),
	help_tc("Light Blue", "node is of kind 'Cast'", NodeColors::Lookup::LIGHT_BLUE),
	help_tc("Pink", "node is of kind 'Comparison'", NodeColors::Lookup::PINK),
	help_tc("Green", "node is of kind 'Branch'", NodeColors::Lookup::GREEN),
	help_tc("Orange", "node is of kind 'Action'", NodeColors::Lookup::ORANGE),
	help_tc("Violet", "node is of kind 'Math'", NodeColors::Lookup::VIOLET),
};

// BETTER_ENUM(NODE_KIND, int, EMPTY = 1, VARIABLE, OPERATION, CAST, COMPARISON, BRANCH, ACTION)
Help::pair_t Help::v_dictionary_items
{
	help_p("Connection", "links the data between nodes"),
	help_p("Node", "basic building block that can be connected with each other"),
	help_p("Variable Node", "used for storing arbitrary value"),
	help_p("Operation Node", "used for performing basic operation on values such as:\naddition, subtraction, multiplication, and division"),
	help_p("Cast Node", "used for converting one data type to another"),
	help_p("Comparison Node", "used for comparing values using:\nequal, not equal, less than, greater than,\nless than or equals to, greater than or equals to,\nor, and and"),
	help_p("Branch Node", "used for redirecting evaluation flow based on the boolean input.\nThis works the same as an if-statement"),
	help_p("Action Node", "used for executing higher-level commands such as:\nprinting text to screen, prompting for user input, and more"),
	help_p("Math Node", "used for performing Math functions and getting numbers such as:\nroot, power, sine, cosine, and tangent"),

	help_p("Slots", "input or output ID used for connection of nodes.\nOnly matching slot type can be connected"),
	help_p("Boolean Slot", "data type for true or false value.\nOnly zero value is evaluated as 'false', otherwise it is 'true'"),
	help_p("Integer Slot", "data type for integer or whole number\n(no decimal)"),
	help_p("Float Slot", "data type for single-precision number\n(can store more integer data type)"),
	help_p("Double Slot", "data type for double-precision number\n(can store more than float data type)"),
	help_p("String Slot", "data type for a series or array of characters"),
};

Help::pair_t Help::v_support_items
{
	help_p(ICON_FA_ENVELOPE " E-Mail:", "mailto:flamendless8@gmail.com"),
	help_p(ICON_FA_GITHUB " Github:", "https://github.com/flamendless/CodeNect-VPS"),
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

		Help::draw_interface();
		ImGui::Separator();

		Help::draw_dictionary();
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
	if (ImGui::TreeNode(ICON_FA_KEYBOARD " Commands"))
	{
		if (ImGui::BeginTable("TableCommands", 3))
		{
			for (const help_t &item : Help::v_key_items)
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
		ImGui::TreePop();
	}
}

void Help::draw_interface(void)
{
	if (ImGui::TreeNode(ICON_FA_SITEMAP " Interfaces"))
	{
		if (ImGui::TreeNode("Connection Colors"))
		{
			if (ImGui::BeginTable("TableConnectionColors", 2, ImGuiTableFlags_Borders))
			{
				for (const help_tc &item : Help::v_connection_colors_items)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextColored(std::get<2>(item), "%s", std::get<0>(item));
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::get<1>(item));
				}
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Node Colors"))
		{
			if (ImGui::BeginTable("TableNodeColors", 2, ImGuiTableFlags_Borders))
			{
				for (const help_tc &item : Help::v_node_colors_items)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextColored(std::get<2>(item), "%s", std::get<0>(item));
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::get<1>(item));
				}
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void Help::draw_dictionary(void)
{
	if (ImGui::TreeNode(ICON_FA_SPELL_CHECK " Dictionary"))
	{
		if (ImGui::BeginTable("TableDictionary", 2, ImGuiTableFlags_Borders))
		{
			for (const help_p &item : Help::v_dictionary_items)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", item.first);
				ImGui::TableNextColumn();
				ImGui::Text("%s", item.second);
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void Help::draw_support(void)
{
	Utils::center_text(ICON_FA_ADDRESS_BOOK " Contacts", true);

	if (ImGui::BeginTable("TableSupport", 3))
	{
		for (const help_p &item : Help::v_support_items)
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
