#include "node_interface.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#   define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <vector>
#include <map>
#include <string>
#include <imgui.h>
#include <imgui_internal.h>
#include "ImNodesEz.h"
#include "plog/Log.h"
#include "IconsFontAwesome5.h"
#include "core/project.hpp"
#include "core/defines.hpp"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "core/font.hpp"
#include "modules/nodes.hpp"
#include "modules/node_logic.hpp"
#include "modules/node_renderer.hpp"
#include "ui/create_node.hpp"

namespace CodeNect
{
bool NodeInterface::is_open = true;
bool NodeInterface::has_changed_theme = false;
ImVec2 NodeInterface::pos;
ImVec2 NodeInterface::size;

ImGuiWindowFlags NodeInterface::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
	ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar;
ImVec2 NodeInterface::center_pos;

const char* NodeInterface::str = ICON_FA_PROJECT_DIAGRAM " Welcome to CodeNect";
const char* NodeInterface::str2 = ICON_FA_ANGLE_LEFT " Hover on the left side to access the sidebar";
const char* NodeInterface::str3 = ICON_FA_TERMINAL " Press <Ctrl+P> to access the command palette";

ImNodes::CanvasState* canvas;

bool NodeInterface::init(void)
{
	const ImVec2 pos = Config::NodeInterface_c::pos;
	const int w = Config::win_width - pos.x * 2;
	const int h = Config::win_height - pos.y * 2;

	NodeInterface::pos = pos;
	NodeInterface::size = ImVec2(w, h);
	NodeInterface::center_pos = ImVec2((float)w/2, (float)h/2);

	canvas = new ImNodes::CanvasState();

	return RES_SUCCESS;
}

void NodeInterface::draw(void)
{
	ImGui::SetNextWindowPos(NodeInterface::pos);
	ImGui::SetNextWindowSize(NodeInterface::size);

	if (ImGui::Begin("NodeInterface", &NodeInterface::is_open, NodeInterface::flags))
	{
		if (!Project::has_open_proj)
			NodeInterface::draw_startup();
		else
			NodeInterface::draw_main();

		ImGui::End();
	}
}

void NodeInterface::draw_startup(void)
{
	Font::use_font(FONT_SIZE::LARGE);
	Utils::center_text(NodeInterface::str, NodeInterface::center_pos, true);
	Utils::center_text(NodeInterface::str2, true);
	Utils::center_text(NodeInterface::str3, true);
	Font::unuse_font();
}

void NodeInterface::draw_main(void)
{
	if (NodeInterface::has_changed_theme)
	{
		canvas = new ImNodes::CanvasState();
		NodeInterface::has_changed_theme = false;
	}

	ImNodes::BeginCanvas(canvas);

	if (Nodes::has_built_meta)
	{
		for (Node* node : Nodes::v_nodes)
			ImNodes::AutoPositionNode(node);

		Nodes::has_built_meta = false;
	}

	NodeInterface::draw_nodes();
	NodeInterface::draw_nodes_context_menu(*canvas);
	CreateNode::draw();
	ImNodes::EndCanvas();
}

void NodeInterface::draw_nodes(void)
{
	NodeLogic::process();

	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin(); it != Nodes::v_nodes.end();)
	{
		Node* node = *it;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, Config::NodeInterface_c::item_inner_spacing);
		ImGui::PushStyleColor(ImGuiCol_Text, Config::NodeInterface_c::title_color);

		if (ImNodes::Ez::BeginNode(node, node->m_kind._to_string(), &node->m_pos, &node->m_selected))
		{
			ImGui::PopStyleColor(1);
			NodeRenderer::draw_node(node);
			//order is important
			ImNodes::Ez::InputSlots(node->m_in_slots.data(), node->m_in_slots.size());
			ImNodes::Ez::OutputSlots(node->m_out_slots.data(), node->m_out_slots.size());
			NodeRenderer::draw_connections(*node);
			NodeRenderer::draw_connected(*node);
		}

		ImNodes::Ez::EndNode();

		ImGui::PopStyleVar(1);

		if (node->m_selected && ImGui::IsKeyPressedMap(ImGuiKey_Delete))
		{
			for (Connection& connection : node->m_connections)
			{
				if (connection.out_node == node)
					((Node*) connection.in_node)->delete_connection(connection);
				else
					((Node*) connection.out_node)->delete_connection(connection);
			}

			node->m_connections.clear();

			//delete node
			Nodes::delete_node(node);
			delete node;
			it = Nodes::v_nodes.erase(it);
		}
		else
			++it;
	}

	if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
	{
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
		ImGui::OpenPopup("NodesContextMenu");
	}
}

void NodeInterface::draw_nodes_context_menu(ImNodes::CanvasState& canvas)
{
	if (ImGui::BeginPopup("NodesContextMenu"))
	{
		if (ImGui::BeginMenu("New Node"))
		{
			if (ImGui::MenuItem("Variable"))
			{
				CreateNode::open(NODE_KIND::VARIABLE);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Operation"))
			{
				CreateNode::open(NODE_KIND::OPERATION);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndMenu();
		}

		for (const std::pair<const std::string, Node*(*)()>& desc : Nodes::m_available_nodes)
		{
			if (ImGui::MenuItem(desc.first.c_str()))
			{
				Node* node = desc.second();
				Nodes::v_nodes.push_back(node);

				switch (node->m_kind)
				{
					case NODE_KIND::EMPTY: break;
					case NODE_KIND::VARIABLE: Nodes::v_nodes_var.push_back((NodeVariable*)node); break;
					case NODE_KIND::OPERATION: Nodes::v_nodes_op.push_back((NodeOperation*)node); break;
					case NODE_KIND::IF: break;
				}

				ImNodes::AutoPositionNode(Nodes::v_nodes.back());
			}
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Reset Zoom"))
            canvas.Zoom = 1;

		if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}
}
