#ifndef _TUTORIAL_HPP
#define _TUTORIAL_HPP

#include <functional>
#include <string>
#include <vector>
#include "imgui.h"

namespace CodeNect
{
struct TutorialInfo
{
	const char* m_title;
	bool m_is_done = false;
	std::string m_content;
	std::function<bool(void)> m_cond;
};

struct Tutorial
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;
	static int current_step;
	static std::vector<TutorialInfo> v_steps;

	static int init(void);
	static void register_commands(void);
	static void toggle(void);
	static void draw(void);
};
}

#endif //_TUTORIAL_HPP
