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

std::string doc_branch_conflict = u8R"(# BRANCH CONFLICT
## EXPLANATION
Think of NodeBranch as a code block. A code block is a block of code that
has a separate stack of memory. Code blocks in C/C++, Java, and other programming
languages are denoted by the combination of curly braces {}. All code that is inside
the curly braces has its own separate variables and execution. Code blocks can
be nested indefinitely. A code block can access variables outside of it as
long as it its not in another separate code block.  This is called scopes.

Branch conflict happens when a variable that is in another code block tries to
access or refer to a variable that is in another code block. For example, if we
declare a variable inside a code block for the 'if-statement', we can not use
that variable in the 'else-statement' because the code block inside 'else-statement'
has no idea or memory of what is declared the 'if-statement'.

## SAMPLE CODE
Here is a C code example:
```
int a = 0; //since 'a' is in the parent scope, both the if and else statements can access this
if (true)
{
	//this is a code block
	int b = 1; //
	printf("value of a: %d\n", a); //we can access 'a'
	printf("value of b: %d\n", b); //we can access 'b' because it is in the same scope
}
else
{
	//this is another code block
	printf("value of a: %d\n", a); //we can access 'a'
	printf("value of b: %d\n", b); //ERROR! this code block does not know the 'b' variable
}
```

## ILLUSTRATION:
Think of a code block as a house. A house has many rooms (code block inside
another code block). Each room can have different utens (variables). The scope
of a house is everything inside of it. The rooms inside a house has access to
the items in another room. But, a house can not access the rooms or items
inside of another house, that would be trespassing!)";

std::vector<const char*> Docs::v_title = {
	"Branch Conflict", "Array Out of Bounds", "Variable Size", "Need Inputs/Lack of Inputs",
	"Comparison Requirements", "Mathematical Function Requirements", "Operation Requirements"
};

//TODO fill up
std::vector<std::string*> Docs::v_doc_id = {
	&doc_branch_conflict,
	&doc_branch_conflict,
	&doc_branch_conflict,
	&doc_branch_conflict,
	&doc_branch_conflict,
	&doc_branch_conflict,
	&doc_branch_conflict,
};

void Docs::register_commands(void)
{
	Command* cmd = new Command("Docs", "open docs window", ICON_FA_BOOK);
	cmd->set_fn(Docs::open);
	cmd->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
}

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

		for (int i = 0; i < 10; i++)
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
