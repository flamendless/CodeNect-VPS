#include "alert.hpp"

#include "plog/Log.h"
#include "IconsFontAwesome5.h"
#include "core/config.hpp"

namespace CodeNect
{
ALERT_TYPE Alert::type;
std::string Alert::message;
bool Alert::is_open = false;
ImGuiWindowFlags Alert::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;

void Alert::open(ALERT_TYPE type, std::string msg)
{
	Alert::type = type;
	Alert::message = msg;
	Alert::is_open = true;
}

void Alert::draw()
{
	if (Alert::is_open)
		ImGui::OpenPopup("AlertPopup");

    ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("AlertPopup", &is_open, flags))
	{
		PLOGD << "here";

		if (type == ALERT_TYPE::SUCCESS)
			ImGui::Text("%s SUCCESS!", ICON_FA_CHECK);
		else if (type == ALERT_TYPE::ERROR)
			ImGui::Text("%s ERROR!", ICON_FA_BUG);

		ImGui::Separator();
		ImGui::Text("%s", Alert::message.c_str());
		ImGui::Separator();

		if (ImGui::Button(ICON_FA_TIMES " Close"))
		{
			Alert::is_open = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
}
