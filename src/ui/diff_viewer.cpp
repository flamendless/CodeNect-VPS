#include "ui/diff_viewer.hpp"

#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/config.hpp"

namespace CodeNect
{
ImGuiWindowFlags DiffViewer::flags =
	ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoSavedSettings |
	ImGuiWindowFlags_AlwaysAutoResize;
bool DiffViewer::is_open = false;
ImVec2 DiffViewer::pos;
ImVec2 DiffViewer::size;
AssessmentResult DiffViewer::cur_result;

int DiffViewer::init(void)
{
	DiffViewer::pos.x = Config::win_width * 0.5;
	DiffViewer::pos.y = Config::win_height * 0.5;
	DiffViewer::size.x = Config::win_width * 0.5;
	DiffViewer::size.y = Config::win_height * 0.5;
	return RES_SUCCESS;
}

void DiffViewer::open(AssessmentResult& res)
{
	DiffViewer::cur_result = res;
	DiffViewer::is_open = true;
}

void DiffViewer::draw(void)
{
	if (!DiffViewer::is_open)
		return;

	ImGui::SetNextWindowPos(DiffViewer::pos, ImGuiCond_Once, ImVec2(0.5, 0.5));
	ImGui::SetNextWindowSize(DiffViewer::size, ImGuiCond_Once);

	if (ImGui::Begin(ICON_FA_TASKS " Diff Viewer", &DiffViewer::is_open, DiffViewer::flags))
	{
		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		DiffViewer::is_open = false;
}
}
