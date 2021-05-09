#ifndef _DOCS_HPP
#define _DOCS_HPP

#include <vector>
#include "imgui.h"
#include "IconsFontAwesome5.h"
#include "core/message_info.hpp"
#include "core/markdown.hpp"

namespace CodeNect
{
struct Doc
{
	const char* m_title;
	std::string m_content;
};

struct Docs
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_doc_open;
	static const char* title;

	static Doc current_doc;

	Docs() = delete;
	static int init(void);
	static void register_commands(void);
	static void open(void);
	static void open_doc_id(DOC_ID);
	static void draw(void);
	static void draw_doc(void);
};
}

#endif //_DOCS_HPP
