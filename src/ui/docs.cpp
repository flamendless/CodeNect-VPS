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
bool Docs::is_doc_open = false;
const char* Docs::title = ICON_FA_BOOK " DOCS";
Doc Docs::current_doc;

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
std::string doc_printing_array =
#include "markdown/doc_printing_array.md"
;
std::string doc_ref_array =
#include "markdown/doc_ref_array.md"
;
std::string doc_iterator =
#include "markdown/doc_iterator.md"
;
std::string doc_for_loops =
#include "markdown/doc_for_loops.md"
;
std::string doc_cyclic =
#include "markdown/doc_cyclic.md"
;

std::map<std::string, Doc> m_docs = {
	{"BRANCH_CONFLICT", {"Branch Conflict", std::move(doc_branch_conflict)}},
	{"AOOB", {"Array Out Of Bounds", std::move(doc_aoob)}},
	{"VAR_SIZE", {"Variable Size", std::move(doc_var_size)}},
	{"NEED_INPUTS", {"Need Inputs/Lack of Inputs", std::move(doc_need_inputs)}},
	{"CMP_REQ", {"Comparison Requirements", std::move(doc_cmp_req)}},
	{"MATH_REQ", {"Mathematical Function Requirements", std::move(doc_math_req)}},
	{"OP_REQ", {"Operation Requirements", std::move(doc_op_req)}},
	{"FOR_LOOP", {"For Loops", std::move(doc_for_loops)}},
	{"ARRAY_TO_PRINT", {"Printing Array", std::move(doc_printing_array)}},
	{"CANT_REF_ARRAY", {"Referencing an Array", std::move(doc_ref_array)}},
	{"IT_MUST_BE_VAR", {"Iterator", std::move(doc_iterator)}},
	{"CYCLIC", {"Cyclic Nodes", std::move(doc_cyclic)}},
};

int Docs::init(void)
{
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

void Docs::open_doc_id(DOC_ID doc_id)
{
	Docs::is_open = true;
	Docs::is_doc_open = true;
	Docs::current_doc = m_docs[doc_id._to_string()];
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
		if (Docs::is_doc_open)
			Docs::draw_doc();
		else
		{
			Utils::center_text(Docs::title, true);
			ImGui::Separator();

			for (std::pair<std::string, Doc> e : m_docs)
			{
				Doc& doc = e.second;
				ImGui::PushID(e.first.c_str());
				if (ImGui::SmallButton(ICON_FA_BOOK_OPEN " open"))
				{
					Docs::is_open = true;
					Docs::is_doc_open = true;
					Docs::current_doc = doc;
				}
				ImGui::SameLine();
				ImGui::Text("%s", doc.m_title);
				ImGui::PopID();
			}

			ImGui::Separator();
			if (ImGui::Button(ICON_FA_TIMES " Close Docs"))
				Docs::is_open = false;
		}
		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
	{
		if (Docs::is_doc_open)
			Docs::is_doc_open = false;
		else
			Docs::is_open = false;
	}
}

void Docs::draw_doc(void)
{
	Utils::center_text(Docs::current_doc.m_title, true);
	ImGui::Separator();
	Markdown::draw(Docs::current_doc.m_content);
	ImGui::Separator();
	if (ImGui::Button(ICON_FA_ARROW_LEFT " Back"))
		Docs::is_doc_open = false;
}
}
