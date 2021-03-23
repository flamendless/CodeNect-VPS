#ifndef _NODE_INTERFACE_HPP
#define _NODE_INTERFACE_HPP

#include "imgui.h"
#include "ImNodes.h"
#include "modules/node.hpp"

namespace CodeNect
{
struct NodeInterface
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool has_changed_theme;
	static ImVec2 pos;
	static ImVec2 size;
	static ImVec2 center_pos;
	static const char* str;
	static const char* str2;
	static const char* str3;

	NodeInterface() = delete;
	static bool init(void);
	static void draw(void);
	static void draw_startup(void);
	static void draw_main(void);
	static void draw_nodes(void);
	static void draw_nodes_context_menu(ImNodes::CanvasState& canvas);
};
}

#endif //_NODE_INTERFACE_HPP
