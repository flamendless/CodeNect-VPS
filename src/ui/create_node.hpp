#ifndef CREATE_NODE_HPP
#define CREATE_NODE_HPP

#include <vector>
#include <string>
#include "imgui.h"
#include "enum.h"
#include "IconsFontAwesome5.h"
#include "modules/node.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
struct Variable
{
	const char* m_name;
	const char* m_comment = "";
	NODE_SLOT m_type = NODE_SLOT::EMPTY;
	std::string m_value;
};

struct NodeTempData
{
	NODE_SLOT slot = NODE_SLOT::EMPTY;
	bool can_create = false;

	char buf_name[BUF_SIZE] = "";
	char buf_string[BUF_SIZE * 2] = "";
	bool valid_name = false;
	bool valid_value = false;
	int res_name;

	NodeValue value;
};

struct CreateNode
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_pos_locked;
	static const char* title;
	static NODE_KIND kind;
	static NodeTempData* data;

	CreateNode() = delete;
	static void open(NODE_KIND kind);
	static void close(void);
	static void create_var_node(void);
	static void draw(void);
	static void draw_buttons(void);
	static void draw_var(void);
	static void draw_opt_bool(void);
	static void draw_opt_int(void);
	static void draw_opt_float(void);
	static void draw_opt_double(void);
	static void draw_opt_string(void);
};
}

#endif //CREATE_NODE_HPP
