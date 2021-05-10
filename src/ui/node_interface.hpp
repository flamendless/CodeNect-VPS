#ifndef _NODE_INTERFACE_HPP
#define _NODE_INTERFACE_HPP

#include "imgui.h"
#include "ImNodes.h"
#include "node/node.hpp"
#include "core/image.hpp"

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
	static Image logo;
	static Node* target_node;
	static bool has_target_node;
	static bool flag_init_setup;
	static bool is_highlighting;
	static bool has_jumped;
	static ImVec4 color_highlight;

	NodeInterface() = delete;
	static bool init(void);
	static void update(float dt);
	static void draw(void);
	static void draw_startup(void);
	static void draw_main(void);
	static void draw_nodes(void);
	static void draw_nodes_context_menu(void);
	static void draw_context_menu(ImNodes::CanvasState& canvas);
	static void jump_to_pos(Node* node);
	static void highlight_node(Node* node);
};
}

#endif //_NODE_INTERFACE_HPP
