#include "ui/info.hpp"

#include "IconsFontAwesome5.h"
#include "core/config.hpp"
#include "modules/debugger.hpp"
#include "ui/terminal.hpp"

namespace CodeNect
{
ImGuiWindowFlags Info::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Info::is_open = false;
ImVec2 Info::pos;

int Info::init(void)
{
	const int x = Config::win_width - 8;
	const int y = 8;
	Info::pos = ImVec2(x, y);
	return RES_SUCCESS;
}

void Info::draw(void)
{
	Info::is_open = Debugger::v_msg_info.size() != 0;

	if (!Info::is_open)
		return;

	ImGui::SetNextWindowPos(Info::pos, ImGuiCond_Always, ImVec2(1, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	if (ImGui::Begin("Info", &Info::is_open, Info::flags))
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1),
			ICON_FA_EXCLAMATION_TRIANGLE " You have a warning\nin your visual code");
		if (ImGui::Button("Click here for more info"))
			Terminal::is_open = true;
	}
	ImGui::End();

	ImGui::PopStyleVar(2);
}
}
