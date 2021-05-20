#include "ui/diff_viewer.hpp"

#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "core/utils.hpp"

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
		Utils::center_text(DiffViewer::cur_result.assessment.title, true);
		if (ImGui::BeginTable("TableDiff", 4, ImGuiTableFlags_SizingFixedFit |
					ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Status");
			ImGui::TableSetupColumn("Line #");
			ImGui::TableSetupColumn("Submitted");
			ImGui::TableSetupColumn("Expected");
			ImGui::TableHeadersRow();

			ImVec4 color = ImVec4(0, 1, 0, 1);
			int ln = 1;
			for (unsigned long i = 0; i < DiffViewer::cur_result.assessment.v_expected.size(); i++)
			{
				ImGui::TableNextRow();
				bool in_conflict = std::find(DiffViewer::cur_result.v_lines_diff.begin(),
						DiffViewer::cur_result.v_lines_diff.end(), (int)i) !=
						DiffViewer::cur_result.v_lines_diff.end();

				if (in_conflict)
					color = ImVec4(0.75f, 0, 0, 1);

				std::string str_submitted = "";
				if (i < DiffViewer::cur_result.assessment.v_submission.size())
					str_submitted = DiffViewer::cur_result.assessment.v_submission[i];
				std::string str_expected = DiffViewer::cur_result.assessment.v_expected[i];

				ImGui::TableNextColumn();
				ImGui::TextColored(color, "%s", (in_conflict ? ICON_FA_TIMES : ICON_FA_CHECK));

				ImGui::TableNextColumn();
				ImGui::TextColored(color, "%d", ln);

				ImGui::TableNextColumn();
				ImGui::TextColored(color, "%s", str_submitted.c_str());

				ImGui::TableNextColumn();
				ImGui::TextColored(color, "%s", str_expected.c_str());
				++ln;
			}

			ImGui::EndTable();
		}
		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		DiffViewer::is_open = false;
}
}
