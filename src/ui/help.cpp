#include "ui/help.hpp"

#include "IconsFontAwesome5Brands.h"
#include "plog/Log.h"
#include "core/commands.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"
#include "core/config.hpp"
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
	help_t(ICON_FA_TERMINAL " Ctrl + Shift + p", "open/close the command palette", "hold the 'ctrl' and 'shift' key then press 'p'"),
	help_t(ICON_FA_CODE " Ctrl + Shift + t", "open/close the terminal", "hold the 'ctrl' and 'shift' key then press 't'"),
	help_t(ICON_FA_SEARCH " Ctrl + Shift + i", "open/close the inspector", "hold the 'ctrl' and 'shift' key then press 'i'"),
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
	help_tc("Blue", "node is of kind 'Variable'/'Data Structure'/'Get'", NodeColors::Lookup::BLUE),
	help_tc("Yellow", "node is of kind 'Operation'", NodeColors::Lookup::YELLOW),
	help_tc("Light Blue", "node is of kind 'Cast'", NodeColors::Lookup::LIGHT_BLUE),
	help_tc("Pink", "node is of kind 'Comparison'", NodeColors::Lookup::PINK),
	help_tc("Green", "node is of kind 'Branch'", NodeColors::Lookup::GREEN),
	help_tc("Orange", "node is of kind 'Action'", NodeColors::Lookup::ORANGE),
	help_tc("Violet", "node is of kind 'Math'", NodeColors::Lookup::VIOLET),
};

std::string Help::str_md_dictionary = u8R"(  * Connection - links the data between nodes
  * Node - basic building block that can be connected with each other
  * Variable Node - used for storing arbitrary value
  * Operation Node - used for performing basic operation on values such as:
    * addition
    * subtraction
    * multiplication
  * division
  * Cast Node - used for converting one data type to another
  * Comparison Node - used for comparing values using:
    * equal
    * not equal
    * less than
    * greater than
    * less than or equals to
    * greater than or equals to
    * or
    * and
  * Branch Node - used for redirecting evaluation flow based on the boolean input. This works the same as an if-statement
  * Action Node - used for executing higher-level commands such as
    * printing text to screen
    * prompting for user input
  * Math Node - used for performing Math functions and getting numbers such as:
    * root
    * power
    * sine
    * cosine
    * tangent
  * Data Structure Node - used for creating data structure like array
  * Get Node - used for getting/accessing value like from array, size of array, and length of string
  * Slots - input or output ID used for connection of nodes. Only matching slot type can be connected
  * Boolean Slot - data type for true or false value. Only zero value is evaluated as 'false', otherwise it is 'true'
  * Integer Slot - data type for integer or whole number (no decimal)
  * Float Slot - data type for single-precision number (can store more integer data type)
  * Double Slot - data type for double-precision number (can store more than float data type)
  * String Slot - data type for a series or array of characters
)";

std::string Help::str_md_libs = u8R"(  * [better-enums](https://github.com/aantron/better-enums)
  * [ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)
  * [fmt](https://github.com/fmtlib/fmt)
  * [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)
  * [DearImGui](https://github.com/ocornut/imgui)
  * [ImGuiMarkdown](https://github.com/juliettef/imgui_markdown)
  * [ImNodes](https://github.com/rokups/ImNodes)
  * [nativefiledialog](https://github.com/mlabbe/nativefiledialog/)
  * [plog](https://github.com/SergiusTheBest/plog)
  * [PPK_ASSERT](https://github.com/gpakosz/PPK_ASSERT)
  * [simpleini](https://github.com/brofield/simpleini/)
  * [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
  * [TinyC Compiler](https://bellard.org/tcc)
  * [Tweeny](https://github.com/mobius3/tweeny)
  * [CppVerbalExpressions](https://github.com/VerbalExpressions/CppVerbalExpressions/)
)";

std::string Help::str_md_support =
	ICON_FA_ENVELOPE "  "
	u8R"([E-Mail](mailto:flamendless8@gmail.com))"
	"\n"
	ICON_FA_GITHUB "  "
	u8R"([CodeNect Repository](https://github.com/flamendless/CodeNect-VPS))";

void md_format_cb(const ImGui::MarkdownFormatInfo& md_info, bool start)
{
	ImGui::defaultMarkdownFormatCallback(md_info, start);
}

void md_link_cb(ImGui::MarkdownLinkCallbackData data)
{
	std::string url(data.link, data.linkLength);
	if (!data.isImage)
		Utils::open_url(url.c_str());
}

void Help::markdown(const std::string& str_md)
{
	Font::md_config.linkCallback = md_link_cb;
	Font::md_config.tooltipCallback = NULL;
	Font::md_config.imageCallback = NULL;
	Font::md_config.linkIcon = ICON_FA_LINK;
	Font::md_config.headingFormats[0] = { Font::H1, true };
	Font::md_config.headingFormats[1] = { Font::H2, true };
	Font::md_config.headingFormats[2] = { Font::H3, false };
	Font::md_config.userData = NULL;
	Font::md_config.formatCallback = md_format_cb;
	ImGui::Markdown(str_md.c_str(), str_md.length(), Font::md_config);
}

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
	ImGui::SetNextWindowSize(ImVec2(640, 480));

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

		if (ImGui::TreeNode(ICON_FA_SPELL_CHECK " Dictionary"))
		{
			Help::markdown(Help::str_md_dictionary);
			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode(ICON_FA_TOOLS " Libraries/Tools Used"))
		{
			Help::markdown(Help::str_md_libs);
			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode(ICON_FA_ADDRESS_BOOK " Support/Contacts"))
		{
			Help::markdown(Help::str_md_support);
			ImGui::TreePop();
		}
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
}
