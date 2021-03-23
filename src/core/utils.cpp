#include "core/utils.hpp"

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
}