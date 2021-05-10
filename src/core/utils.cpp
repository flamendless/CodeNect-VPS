#include "core/utils.hpp"

#include <algorithm>

namespace CodeNect::Utils
{
bool exists_in_slots(NODE_SLOT slot, std::vector<NODE_SLOT> slots)
{
	for (NODE_SLOT& _slot : slots)
	{
		if (_slot == slot)
			return true;
	}

	return false;
}

void center_text(const char* txt)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const float w = ImGui::GetWindowContentRegionWidth();

	ImGui::SetCursorPosX(w/2 - txt_size.x/2);
}

void center_text(const char* txt, bool draw_text)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const float w = ImGui::GetWindowContentRegionWidth();

	ImGui::SetCursorPosX(w/2 - txt_size.x/2);

	if (draw_text)
		ImGui::Text("%s", txt);
}

void center_text(const char* txt, const ImVec2& offset_pos)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const ImVec2 pos = ImVec2(offset_pos.x - txt_size.x/2, offset_pos.y - txt_size.y/2);

	ImGui::SetCursorPos(pos);
}

void center_text(const char* txt, const ImVec2& offset_pos, bool draw_text)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const ImVec2 pos = ImVec2(offset_pos.x - txt_size.x/2, offset_pos.y - txt_size.y/2);

	ImGui::SetCursorPos(pos);

	if (draw_text)
		ImGui::Text("%s", txt);
}

void help_marker(const char* desc, bool same_line)
{
	if (same_line)
		ImGui::SameLine();

	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void display_asterisk(bool ref, const char* str)
{
	if (ref)
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "*");
		ImGui::SameLine();

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", str);
	}
	else
	{
		ImGui::TextColored(ImVec4(0, 0, 0, 0), "*");
		ImGui::SameLine();
	}
}

void display_asterisk(bool ref)
{
	display_asterisk(ref, "This field is required");
}

std::string time_now(void)
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

	return std::string(buffer);
}

void open_url(const char* url)
{
	std::string str_url = url;
#ifdef OS_LINUX
	std::string cmd = "xdg-open " + str_url;
	system(cmd.c_str());
#elif OS_WIN
#endif
}

bool bool_from_string(const char* str)
{
	if (std::strcmp(str, "true") == 0)
		return true;
	else if (std::strcmp(str, "false") == 0)
		return false;

	return false;
}

void string_to_name(std::string& str)
{
	//converts ENUM string to readable string
	//example: ARRAY_ACCESS to Array Access
	bool prev_was_ws = false;
	std::transform(str.begin() + 1, str.end(), str.begin() + 1,
		[&prev_was_ws](unsigned char c)
		{
			if (c == '_')
			{
				prev_was_ws = true;
				return (int)' ';
			}

			if (prev_was_ws)
			{
				prev_was_ws = false;
				return std::toupper(c);
			}

			return std::tolower(c);
		});
}

const char* cmp_to_op_str(NODE_CMP& cmp)
{
	switch (cmp)
	{
		case NODE_CMP::EMPTY: break;
		case NODE_CMP::EQ: return "=="; break;
		case NODE_CMP::NEQ: return "!="; break;
		case NODE_CMP::LT: return "<"; break;
		case NODE_CMP::GT: return ">"; break;
		case NODE_CMP::LTE: return "<="; break;
		case NODE_CMP::GTE: return ">="; break;
		case NODE_CMP::OR: return "||"; break;
		case NODE_CMP::AND: return "&&"; break;
	}
	return "";
}

bool validate_for(int si, int ei, int inc, NODE_CMP cmp)
{
	bool is_valid = false;

	if (inc == 0)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " a 0 increment may result in infinite loop");

	switch (cmp)
	{
		case NODE_CMP::EMPTY: break;
		case NODE_CMP::EQ: break;
		case NODE_CMP::NEQ: break;
		case NODE_CMP::LT:
		{
			//for (int i = 0; i < 10; i++) //good
			//for (int i = 10; i < 0; i++) //not
			//for (int i = 0; i < 10; i--) //inf
			//for (int i = 10; i < 0; i--) //not
			if (inc > 0)
			{
				if (si < ei)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			else if (inc < 0)
			{
				if (si < ei)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			break;
		}

		case NODE_CMP::LTE:
		{
			//for (int i = 0; i <= 10; i++) //good
			//for (int i = 10; i <= 0; i++) //not
			//for (int i = 0; i <= 10; i--) //inf
			//for (int i = 10; i <= 0; i--) //not
			if (inc > 0)
			{
				if (si <= ei)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			else if (inc < 0)
			{
				if (si <= ei)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			break;
		}

		case NODE_CMP::GT:
		{
			//for (int i = 0; i > 10; i++) //not
			//for (int i = 10; i > 0; i++) //inf
			//for (int i = 0; i > 10; i--) //not
			//for (int i = 10; i > 0; i--) //good
			if (inc > 0)
			{
				if (si > ei)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			else if (inc < 0)
			{
				if (si > ei)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			break;
		}

		case NODE_CMP::GTE:
		{
			//for (int i = 0; i >= 10; i++) //not
			//for (int i = 10; i >= 0; i++) //inf
			//for (int i = 0; i >= 10; i--) //not
			//for (int i = 10; i >= 0; i--) //good
			if (inc > 0)
			{
				if (si >= ei)
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this will result in infinite loop");
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			else if (inc < 0)
			{
				if (si >= ei)
					is_valid = true;
				else
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ICON_FA_EXCLAMATION_TRIANGLE " this iteration will not be triggered even once");
			}
			break;
		}
	}

	return is_valid;
}
}
