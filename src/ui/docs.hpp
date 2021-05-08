#ifndef _DOCS_HPP
#define _DOCS_HPP

#include <vector>
#include "imgui.h"
#include "IconsFontAwesome5.h"
#include "core/message_info.hpp"
#include "core/markdown.hpp"

namespace CodeNect
{
struct Docs
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static const char* title;

	static std::vector<const char*> v_title;
	static std::vector<std::string> v_doc_id;

	Docs() = delete;
	static int init(void);
	static void register_commands(void);
	static void open(void);
	static void open_doc_id(DOC_ID&);
	static void draw(void);
	static void draw_doc(void);
};
}

#endif //_DOCS_HPP
