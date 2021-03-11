#ifndef UTILS_HPP
#define UTILS_HPP

#include <ctime>
#include <string>
#include "imgui.h"

namespace CodeNect
{
namespace Utils
{
inline void center_text(const char* txt)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const float w = ImGui::GetWindowContentRegionWidth();

	ImGui::SetCursorPosX(w/2 - txt_size.x/2);
}

inline void center_text(const char* txt, bool draw_text)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const float w = ImGui::GetWindowContentRegionWidth();

	ImGui::SetCursorPosX(w/2 - txt_size.x/2);

	if (draw_text)
		ImGui::Text("%s", txt);
}

inline void center_text(const char* txt, const ImVec2& offset_pos)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const ImVec2 pos = ImVec2(offset_pos.x - txt_size.x/2, offset_pos.y - txt_size.y/2);

	ImGui::SetCursorPos(pos);
}

inline void center_text(const char* txt, const ImVec2& offset_pos, bool draw_text)
{
	const ImVec2 txt_size = ImGui::CalcTextSize(txt);
	const ImVec2 pos = ImVec2(offset_pos.x - txt_size.x/2, offset_pos.y - txt_size.y/2);

	ImGui::SetCursorPos(pos);

	if (draw_text)
		ImGui::Text("%s", txt);
}

inline std::string time_now(void)
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

	return std::string(buffer);
}

inline void open_url(const char* url)
{
	std::string str_url = url;
#ifdef OS_LINUX
	std::string cmd = "xdg-open " + str_url;
	system(cmd.c_str());
#elif OS_WIN
#endif
}
}
}

#endif //UTILS_HPP
