#ifndef _DIFF_VIEWER_HPP
#define _DIFF_VIEWER_HPP

#include "imgui.h"
#include "modules/assessments.hpp"

namespace CodeNect
{
struct DiffViewer
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;
	static AssessmentResult cur_result;

	DiffViewer() = delete;
	static int init(void);
	static void open(AssessmentResult&);
	static void draw(void);
};
}

#endif //_DIFF_VIEWER_HPP
