#ifndef _TERMINAL_HPP
#define _TERMINAL_HPP

#include "imgui.h"
#include "TextEditor.h"
#include "core/message_info.hpp"

namespace CodeNect
{
enum class T_MODE { OUTPUT = 1, CODE };

struct Terminal
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static ImVec2 pos;
	static ImVec2 size;
	static T_MODE mode;
	static TextEditor editor;
	static TextEditor::LanguageDefinition lang_c;

	Terminal() = delete;
	static int init(void);
	static void register_commands(void);
	static void toggle(void);
	static bool keypress(int key, int scancode, int mods);
	static void draw(void);
	static void draw_message_info(std::vector<MessageInfo>& v);
	static void draw_assessment(void);
	static void draw_output(void);
	static void draw_code(void);
};
}

#endif //_TERMINAL_HPP
