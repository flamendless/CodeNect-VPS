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

std::vector<const char*> Docs::v_title = {
	//for transpiler/node_logic warnings/errors
	"Branch Conflict", "Array Out of Bounds", "Variable Size", "Need Inputs/Lack of Inputs",
	"Comparison Requirements", "Mathematical Function Requirements", "Operation Requirements",

	//for connection warning/errors
	"Printing Array", "Referencing an Array", "Iterator"
};

//TODO fill up
std::vector<std::string> Docs::v_doc_id = {
	std::move(doc_branch_conflict),
	std::move(doc_aoob),
	std::move(doc_var_size),
	std::move(doc_need_inputs),
	std::move(doc_cmp_req),
	std::move(doc_math_req),
	std::move(doc_op_req),
	std::move(doc_printing_array),
	std::move(doc_ref_array),
	std::move(doc_iterator),
};
std::string opened_doc_title;
std::string opened_doc_str;
bool is_doc_open = false;

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
	is_doc_open = true;
	Docs::is_open = true;
	opened_doc_title = std::string(v_title[doc_id._to_index()]);
	opened_doc_str = v_doc_id[doc_id._to_index()];
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
		if (is_doc_open)
			Docs::draw_doc();
		else
		{
			Utils::center_text(Docs::title, true);
			ImGui::Separator();
			for (int i = 0; i < v_doc_id.size(); i++)
			{
				ImGui::PushID(i);
				if (ImGui::SmallButton(ICON_FA_BOOK_OPEN " open"))
				{
					is_doc_open = true;
					opened_doc_title = std::string(v_title[i]);
					opened_doc_str = v_doc_id[i];
				}
				ImGui::SameLine();
				ImGui::Text("%s", v_title[i]);
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
		opened_doc_title.clear();
		opened_doc_str.clear();
		if (is_doc_open)
			is_doc_open = false;
		else
			Docs::is_open = false;
	}
}

void Docs::draw_doc(void)
{
	Utils::center_text(opened_doc_title.c_str(), true);
	ImGui::Separator();
	Markdown::draw(opened_doc_str);
	ImGui::Separator();
	if (ImGui::Button(ICON_FA_ARROW_LEFT " Back"))
	{
		is_doc_open = false;
		opened_doc_title.clear();
		opened_doc_str.clear();
	}
}
}
