#include "ui/terminal.hpp"

#include <imgui_internal.h>
#include "fmt/format.h"
#include "IconsFontAwesome5.h"
#include "core/config.hpp"
#include "core/commands.hpp"
#include "core/font.hpp"
#include "core/utils.hpp"
#include "modules/input.hpp"
#include "modules/transpiler.hpp"
#include "modules/debugger.hpp"
#include "node/node_colors.hpp"
#include "ui/node_interface.hpp"
#include "ui/create_node.hpp"
#include "ui/docs.hpp"

#if DEBUG_MODE
#include <fstream>
#include <fmt/chrono.h>
#endif

namespace CodeNect
{
ImGuiWindowFlags Terminal::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_AlwaysAutoResize;
bool Terminal::is_open = false;
ImVec2 Terminal::pos;
ImVec2 Terminal::size;
T_MODE Terminal::mode = T_MODE::OUTPUT;
TextEditor Terminal::editor;
TextEditor::LanguageDefinition Terminal::lang_c;

int Terminal::init(void)
{
	const int x = (float)Config::win_width/2;
	const int y = (float)Config::win_height/2;

	Input::register_key_event(Terminal::keypress);

	Terminal::pos = ImVec2(x, y);
	Terminal::size = ImVec2(640, 480);

	Terminal::lang_c = TextEditor::LanguageDefinition::C();
	Terminal::editor.SetLanguageDefinition(Terminal::lang_c);
	Terminal::editor.SetReadOnly(true);
	Terminal::editor.SetHandleKeyboardInputs(false);

	return RES_SUCCESS;
}

void Terminal::register_commands(void)
{
	Command* cmd = new Command("Terminal", "open/close the terminal", ICON_FA_CODE);
	cmd->set_fn(Terminal::toggle);
	cmd->m_close_command_palette = true;
	Commands::register_cmd(*cmd);
}

void Terminal::toggle(void)
{
	Terminal::is_open = !Terminal::is_open;
}

bool Terminal::keypress(int key, int, int mods)
{
	if (key == GLFW_KEY_T && mods == (GLFW_MOD_SHIFT | GLFW_MOD_CONTROL))
		Terminal::toggle();

	return false;
}

void Terminal::draw(void)
{
	if (!Terminal::is_open)
		return;

	ImGui::SetNextWindowPos(Terminal::pos, ImGuiCond_Once, ImVec2(0.5, 0.5));
	ImGui::SetNextWindowSize(Terminal::size);

	if (ImGui::Begin("Terminal", &Terminal::is_open, Terminal::flags))
	{
		Font::use_font(FONT_SIZE::LARGE);
		Utils::center_text(ICON_FA_CODE " Terminal", true);
		Font::unuse_font();
		Utils::center_text("press <ESC> to close", true);
		ImGui::Separator();

		ImGui::Text("Tabs:");
		ImGui::SameLine();
		if (ImGui::SmallButton("Output"))
			Terminal::mode = T_MODE::OUTPUT;
		ImGui::SameLine();
		if (ImGui::SmallButton("Code"))
			Terminal::mode = T_MODE::CODE;

		ImGui::Separator();

		switch (Terminal::mode)
		{
			case T_MODE::OUTPUT: Terminal::draw_output(); break;
			case T_MODE::CODE: Terminal::draw_code(); break;
		}

		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_C))
		Transpiler::compile();
	else if (ImGui::IsKeyPressedMap(ImGuiKey_C))
		Transpiler::run();
	else if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		Terminal::is_open = false;
}

void Terminal::draw_message_info(std::vector<MessageInfo>& v)
{
	int i = 0;
	for (MessageInfo& msg : v)
	{
		switch (msg.m_type)
		{
			case OUTPUT_TYPE::NORMAL: ImGui::Text("%s", msg.m_msg.c_str()); break;
			case OUTPUT_TYPE::SUCCESS:
			{
				ImGui::TextColored(NodeColors::Lookup::GREEN, "%s", msg.m_msg.c_str());
				break;
			}
			case OUTPUT_TYPE::ERR:
			{
				ImGui::TextColored(NodeColors::Lookup::RED, ICON_FA_TIMES_CIRCLE " %s", msg.m_msg.c_str());
				break;
			}
			case OUTPUT_TYPE::WARNING:
			{
				ImGui::TextColored(NodeColors::Lookup::YELLOW, ICON_FA_EXCLAMATION_TRIANGLE " %s", msg.m_msg.c_str());
				break;
			}
			case OUTPUT_TYPE::PROMPT:
			{
				ImGui::TextColored(NodeColors::Lookup::ORANGE, "%s", msg.m_msg.c_str());
				break;
			}
		}

		if (msg.m_node)
		{
			ImGui::SameLine();
			ImGui::Text("%s", msg.m_node->m_name);
			ImGui::SameLine();
			ImGui::PushID(i);

			if (ImGui::SmallButton(ICON_FA_SEARCH))
			{
				PLOGD << msg.m_node;
				PLOGD << msg.m_node->m_name;
				NodeInterface::jump_to_pos(msg.m_node);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Jump the camera's position to the node's position");

			ImGui::SameLine();
			if (ImGui::SmallButton(ICON_FA_EDIT))
				CreateNode::edit(msg.m_node);
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Edit node");

			if (msg.m_doc_id != +DOC_ID::EMPTY)
			{
				ImGui::SameLine();
				if (ImGui::SmallButton(ICON_FA_BOOK))
					Docs::open_doc_id(msg.m_doc_id);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("View more info in the Docs");
			}
			ImGui::PopID();
		}
		i++;
	}
}

void Terminal::draw_output(void)
{
	Utils::center_text(ICON_FA_TERMINAL " OUTPUT", true);
	if (ImGui::SmallButton("Compile"))
		Transpiler::compile();
	ImGui::SameLine();
	if (ImGui::SmallButton("Run"))
		Transpiler::run();
	ImGui::SameLine();
	if (ImGui::SmallButton("Clear"))
		Transpiler::clear();

#if DEBUG_MODE
	ImGui::SameLine();
	if (ImGui::SmallButton("Save"))
	{
		if (Transpiler::runnable_code.length() != 0)
		{
			std::time_t t = std::time(nullptr);
			std::string out_filename = fmt::format("out/out_file_{:%Y_%m_%d_%I_%M_%s}.c", fmt::localtime(t));
			std::ofstream out_file(out_filename);
			out_file << Transpiler::runnable_code;
			out_file.close();
			std::string msg = "Saved to " + out_filename;
			Transpiler::add_message(std::move(msg));
		}
	}
#endif
	ImGui::Separator();

	if (Debugger::v_msg_info.size() != 0)
	{
		ImGui::TextColored(NodeColors::Lookup::YELLOW, ICON_FA_EXCLAMATION_TRIANGLE " %s", "There are warnings with your node structure");
		ImGui::TextColored(NodeColors::Lookup::YELLOW, ICON_FA_EXCLAMATION_TRIANGLE " %s", "This may result in error with the transpiled code");
		ImGui::TextColored(NodeColors::Lookup::YELLOW, ICON_FA_EXCLAMATION_TRIANGLE " %s", "You may check the Docs for more info regarding the warning or error");
		Terminal::draw_message_info(Debugger::v_msg_info);
	}

	Terminal::draw_message_info(Transpiler::v_output);
}

void Terminal::draw_code(void)
{
	const int length = Transpiler::output_code.length();
	Utils::center_text(ICON_FA_FILE_CODE " CODE", true);

	if (length == 0)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

	if (ImGui::SmallButton("Save as"))
	{
		if (length != 0)
			Transpiler::save_file();
	}

	ImGui::SameLine();
	if (ImGui::SmallButton("Copy"))
		Terminal::editor.Copy();

	ImGui::SameLine();
	if (ImGui::SmallButton("Select All"))
	{
		Terminal::editor.SetSelection(TextEditor::Coordinates(),
			TextEditor::Coordinates(Terminal::editor.GetTotalLines(), 0));
	}

	if (length == 0)
		ImGui::PopStyleVar(1);

	ImGui::Separator();
	Terminal::editor.Render("TextEditor");
}
}
