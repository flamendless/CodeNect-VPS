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
#include "node/nodes.hpp"
#include "node/node_logic.hpp"
#include "node/node_renderer.hpp"
#include "ui/create_node.hpp"
#include "ui/zoom.hpp"
#include "modules/filesystem.hpp"

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

std::vector<const char*> NodeInterface::v_str = {
	ICON_FA_PROJECT_DIAGRAM " Welcome to CodeNect",
	ICON_FA_ANGLE_LEFT " Hover on the left side to access the sidebar",
	ICON_FA_TERMINAL " Press <Ctrl+Shift+p> to access the command palette",
	ICON_FA_CODE " Press <Ctrl+Shift+t> to open/hide the terminal",
	ICON_FA_SEARCH " Press <Ctrl+Shift+i> to open/hide the inspector",
};
Image NodeInterface::logo;
ImVec2 NodeInterface::target_node_pos;
bool NodeInterface::has_target_node = false;

Node* current_node;
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
	Filesystem::load_texture_from_file("assets/logo.png", NodeInterface::logo);

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

	ImGui::SetCursorPosX(NodeInterface::center_pos.x - NodeInterface::logo.size.x/2);
	ImGui::SetCursorPosY(NodeInterface::center_pos.y - NodeInterface::logo.size.y);
	ImGui::Image(NodeInterface::logo.get_texture(),
		NodeInterface::logo.size, ImVec2(0, 0), ImVec2(1, 1),
		ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

	for (const char* str : NodeInterface::v_str)
	{
		Utils::center_text(str, true);
	}

	Font::unuse_font();
}

void NodeInterface::draw_main(void)
{
	if (NodeInterface::has_changed_theme)
	{
		canvas = new ImNodes::CanvasState();
		NodeInterface::has_changed_theme = false;
	}

	if (NodeInterface::has_target_node)
	{
		float tx = -NodeInterface::target_node_pos.x + NodeInterface::size.x/4;
		float ty = -NodeInterface::target_node_pos.y + NodeInterface::size.y/4;
		canvas->Offset.x = tx;
		canvas->Offset.y = ty;
		NodeInterface::has_target_node = false;
	}

	ImNodes::BeginCanvas(canvas);
	NodeInterface::draw_nodes();
	NodeInterface::draw_nodes_context_menu();
	NodeInterface::draw_context_menu(*canvas);
	CreateNode::draw();
	Zoom::zoom_factor = canvas->Zoom;
	ImNodes::EndCanvas();
}

void NodeInterface::draw_nodes(void)
{
	NodeLogic::process();

	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin(); it != Nodes::v_nodes.end();)
	{
		Node* node = *it;

		//style node
		NodeRenderer::push_node_style();
		NodeRenderer::push_node_color(node);

		const char* title = Nodes::get_title(node);
		if (ImNodes::Ez::BeginNode(node, title, &node->m_pos, &node->m_selected))
		{
			NodeRenderer::pop_node_color();
			NodeRenderer::draw_node(node);
			//order is important
			ImNodes::Ez::InputSlots(node->m_in_slots.data(), node->m_in_slots.size());
			ImNodes::Ez::OutputSlots(node->m_out_slots.data(), node->m_out_slots.size());
			NodeRenderer::draw_connections(*node);
		}

		ImNodes::Ez::EndNode();
		NodeRenderer::pop_node_style();

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			current_node = node;
			node->m_selected = true;
			ImGui::FocusWindow(ImGui::GetCurrentWindow());
			ImGui::OpenPopup("NodesContextMenu");
		}

		if (node->m_to_delete || (node->m_selected && ImGui::IsKeyPressedMap(ImGuiKey_Delete)))
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
			Nodes::delete_node(it);
			current_node = nullptr;
		}
		else
			++it;
	}

	if (ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(1))
	{
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
		ImGui::OpenPopup("NodesInterfaceContextMenu");
	}
}

void NodeInterface::draw_nodes_context_menu(void)
{
	if (ImGui::BeginPopup("NodesContextMenu"))
	{
		if (ImGui::MenuItem("Edit"))
		{
			CreateNode::edit(current_node);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Delete"))
			current_node->m_to_delete = true;

#if DEBUG_MODE
		if (ImGui::MenuItem("Print Name"))
		{
			PLOGD << current_node->m_name;
		}
#endif

		ImGui::EndPopup();
	}
}

void NodeInterface::draw_context_menu(ImNodes::CanvasState& canvas)
{
	if (ImGui::BeginPopup("NodesInterfaceContextMenu"))
	{
		if (ImGui::BeginMenu("Basic Node"))
		{
			for (NODE_KIND kind : NODE_KIND::_values())
			{
				if (kind == +NODE_KIND::EMPTY ||
					kind == +NODE_KIND::ACTION ||
					kind == +NODE_KIND::MATH ||
					kind == +NODE_KIND::DS ||
					kind == +NODE_KIND::GET)
					continue;

				std::string item = kind._to_string();
				Utils::string_to_name(item);

				if (ImGui::MenuItem(item.c_str()))
				{
					CreateNode::open(kind);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Action Node"))
		{
			for (NODE_ACTION action : NODE_ACTION::_values())
			{
				if (action == +NODE_ACTION::EMPTY)
					continue;

				std::string item = action._to_string();
				Utils::string_to_name(item);

				if (ImGui::MenuItem(item.c_str()))
				{
					CreateNode::open_action(NODE_KIND::ACTION, action);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Math Node"))
		{
			for (NODE_MATH math : NODE_MATH::_values())
			{
				if (math == +NODE_MATH::EMPTY)
					continue;

				std::string item = math._to_string();
				Utils::string_to_name(item);

				if (ImGui::MenuItem(item.c_str()))
				{
					CreateNode::open_math(NODE_KIND::MATH, math);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Data Structure Node"))
		{
			for (NODE_DS ds : NODE_DS::_values())
			{
				if (ds == +NODE_DS::EMPTY)
					continue;

				std::string item = ds._to_string();
				Utils::string_to_name(item);

				if (ImGui::MenuItem(item.c_str()))
				{
					CreateNode::open_ds(NODE_KIND::DS, ds);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Get"))
		{
			for (NODE_GET get : NODE_GET::_values())
			{
				if (get == +NODE_GET::EMPTY)
					continue;

				std::string item = get._to_string();
				Utils::string_to_name(item);

				if (ImGui::MenuItem(item.c_str()))
				{
					CreateNode::open_get(NODE_KIND::GET, get);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndMenu();
		}

		for (const std::pair<const std::string, Node*(*)()>& desc : Nodes::m_available_nodes)
		{
			if (ImGui::MenuItem(desc.first.c_str()))
			{
				Node* node = desc.second();
				Nodes::v_nodes.push_back(node);
				ImNodes::AutoPositionNode(Nodes::v_nodes.back());
			}
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Reset Zoom"))
			canvas.Zoom = 1;

		if (ImGui::MenuItem("Reset View Position"))
		{
			canvas.Offset.x = 0;
			canvas.Offset.y = 0;
		}

		if (ImGui::IsAnyMouseDown() && !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}
}
