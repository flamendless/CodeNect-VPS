#include "ui/docs.hpp"

#include "imgui_internal.h"
#include "plog/Log.h"
#include "core/defines.hpp"
#include "core/commands.hpp"

namespace CodeNect
{
ImGuiWindowFlags Docs::flags =
	ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_AlwaysAutoResize;
bool Docs::is_open = false;
const char* Docs::title = ICON_FA_BOOK " DOCS";

std::array<bool, DOC_ID::_size()> is_open_doc_id = { false };

std::string doc_branch_conflict =
#include "markdown/doc_branch_conflict.md"
;
std::string doc_aoob =
#include "markdown/doc_aoob.md"
;
std::string doc_var_size =
#include "markdown/doc_var_size.md"
;
std::string doc_need_inputs =
#include "markdown/doc_need_inputs.md"
;
std::string doc_cmp_req =
#include "markdown/doc_cmp_req.md"
;
std::string doc_math_req =
#include "markdown/doc_math_req.md"
;
std::string doc_op_req =
#include "markdown/doc_op_req.md"
;

std::vector<const char*> Docs::v_title = {
	"Branch Conflict", "Array Out of Bounds", "Variable Size", "Need Inputs/Lack of Inputs",
	"Comparison Requirements", "Mathematical Function Requirements", "Operation Requirements"
};

//TODO fill up
std::vector<std::string*> Docs::v_doc_id = {
	&doc_branch_conflict,
	&doc_aoob,
	&doc_var_size,
	&doc_need_inputs,
	&doc_cmp_req,
	&doc_math_req,
	&doc_op_req,
};

int Docs::init(void)
{
	if (Docs::v_title.size() != Docs::v_doc_id.size())
	{
		PLOGE << "v_title size = " << v_title.size();
		PLOGE << "v_doc_id size = " << v_doc_id.size();
		return RES_FAIL;
	}

	return RES_SUCCESS;
}

void Docs::register_commands(void)
{
	Command* cmd = new Command("Docs", "open docs window", ICON_FA_BOOK);
	cmd->set_fn(Docs::open);
	cmd->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
}

void Docs::open(void)
{
	Docs::is_open = true;
	PLOGD << "Opened Docs window";
}

void Docs::open_doc_id(DOC_ID& doc_id)
{
	is_open_doc_id.fill(false);
	Docs::is_open = true;
	is_open_doc_id[doc_id._to_index()] = true;
	PLOGD << "Opened Docs in " << doc_id._to_string();
}

void Docs::draw(void)
{
	if (!Docs::is_open)
		return;

	ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(640, 480));

	if (!ImGui::IsPopupOpen("AlertPopup"))
		ImGui::SetNextWindowFocus();

	if (ImGui::Begin("Docs", &Docs::is_open, Docs::flags))
	{
		Utils::center_text(Docs::title, true);
		ImGui::Separator();

		for (int i = 0; i < v_doc_id.size(); i++)
		{
			if (is_open_doc_id[i + 1])
				ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode(v_title[i]))
			{
				Markdown::draw(*v_doc_id[i]);
				ImGui::TreePop();
			}
			else
				is_open_doc_id[i + 1] = false;

			if (i < v_doc_id.size() - 1)
				ImGui::Separator();
		}

		ImGui::Separator();
		if (ImGui::Button(ICON_FA_TIMES " Close Docs"))
			Docs::is_open = false;
		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		Docs::is_open = false;
}
}
