#ifndef _NODE_INTERFACE_HPP
#define _NODE_INTERFACE_HPP

#include "imgui.h"
#include "ImNodes.h"
#include "node/node.hpp"

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
	static std::vector<const char*> v_str;

	NodeInterface() = delete;
	static bool init(void);
	static void draw(void);
	static void draw_startup(void);
	static void draw_main(void);
	static void draw_nodes(void);
	static void draw_nodes_context_menu(void);
	static void draw_context_menu(ImNodes::CanvasState& canvas);
};
}

#endif //_NODE_INTERFACE_HPP
