#include "ui/terminal.hpp"

#include <imgui_internal.h>
#include "IconsFontAwesome5.h"
#include "core/config.hpp"
#include "core/commands.hpp"
#include "core/font.hpp"
#include "core/utils.hpp"
#include "modules/transpiler.hpp"
#include "node/node_colors.hpp"

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

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		Terminal::is_open = false;
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
	ImGui::Separator();

	for (const std::pair<std::string, OUTPUT_TYPE>& p : Transpiler::v_output)
	{
		switch (p.second)
		{
			case OUTPUT_TYPE::NORMAL: ImGui::Text("%s", p.first.c_str()); break;
			case OUTPUT_TYPE::SUCCESS:
			{
				ImGui::TextColored(NodeColors::Lookup::GREEN, "%s", p.first.c_str());
				break;
			}
			case OUTPUT_TYPE::ERROR:
			{
				ImGui::TextColored(NodeColors::Lookup::RED, "%s", p.first.c_str());
				break;
			}
		}
	}
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

	if (length == 0)
		ImGui::PopStyleVar(1);

	ImGui::Separator();
	Terminal::editor.Render("TextEditor");
}
}
