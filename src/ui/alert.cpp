#include "alert.hpp"

#include "plog/Log.h"
#include "IconsFontAwesome5.h"
#include "core/config.hpp"
#include "core/font.hpp"
#include "core/defines.hpp"
#include "modules/input.hpp"

namespace CodeNect
{
ALERT_TYPE Alert::type;
std::string Alert::message;
bool Alert::is_open = false;
bool Alert::has_cb = false;
DOC_ID Alert::doc_id = DOC_ID::EMPTY;
ImGuiWindowFlags Alert::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
std::function<void(void)> Alert::fn_custom_draw;

int Alert::init(void)
{
	Input::register_key_event(Alert::keypress);

	return RES_SUCCESS;
}

bool Alert::keypress(int key, int scancode, int mods)
{
	if (!Alert::is_open)
		return false;

	if (key == GLFW_KEY_ENTER)
		Alert::close();

	return true;
}

void Alert::open(ALERT_TYPE type, std::string msg)
{
	Alert::type = type;
	Alert::message = msg;
	Alert::is_open = true;
}

void Alert::open_for_docs(std::string msg, DOC_ID doc_id)
{
	Alert::type = ALERT_TYPE::WARNING;
	Alert::message = msg;
	Alert::is_open = true;
	Alert::doc_id = doc_id;
}

void Alert::close(void)
{
	Alert::has_cb = false;
	Alert::doc_id = DOC_ID::EMPTY;
	Alert::fn_custom_draw = nullptr;
	Alert::is_open = false;
	ImGui::CloseCurrentPopup();
}

void Alert::draw(void)
{
	if (Alert::is_open)
		ImGui::OpenPopup("AlertPopup");

    ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("AlertPopup", &is_open, flags))
	{
		switch (type)
		{
			case ALERT_TYPE::SUCCESS: ImGui::Text("%s SUCCESS!", ICON_FA_CHECK); break;
			case ALERT_TYPE::ERR: ImGui::Text("%s ERROR!", ICON_FA_BUG); break;
			case ALERT_TYPE::WARNING: ImGui::Text("%s WARNING!", ICON_FA_EXCLAMATION_CIRCLE); break;
		}

		ImGui::Separator();
		ImGui::Text("%s", Alert::message.c_str());
		ImGui::Separator();

		if (Alert::has_cb)
			Alert::fn_custom_draw();
		else
		{
			if (Alert::doc_id != +DOC_ID::EMPTY)
			{
				if (ImGui::Button("Open in Docs for more info"))
				{
					Docs::open_doc_id(Alert::doc_id);
					Alert::close();
				}
				ImGui::SameLine();
			}

			if (ImGui::Button(ICON_FA_TIMES " Close"))
				Alert::close();
		}

		ImGui::EndPopup();
	}
}
}
