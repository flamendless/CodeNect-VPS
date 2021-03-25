#ifndef _CREATE_NODE_HPP
#define _CREATE_NODE_HPP

#include <variant>
#include <vector>
#include <string>
#include "imgui.h"
#include "enum.h"
#include "IconsFontAwesome5.h"
#include "node/node.hpp"
#include "core/temp_data.hpp"

namespace CodeNect
{
struct CreateNode
{
	static ImGuiWindowFlags flags;
	static bool is_first;
	static bool is_open;
	static bool is_pos_locked;
	static bool is_edit_mode;
	static const char* title;
	static NODE_KIND kind;
	static Node* node_to_edit;
	static bool can_create;
	static std::variant<TempVarData*, TempOperationData*, TempCastData*> data;

	CreateNode() = delete;
	static void open(NODE_KIND kind);
	static void edit(Node* node);
	static void close(void);
	static void create_var_node(void);
	static void create_op_node(void);

	static void draw(void);
	static void draw_desc(void);
	static void draw_buttons(void);

	//variable node
	static void draw_var(void);
	static void draw_opt_bool(void);
	static void draw_opt_int(void);
	static void draw_opt_float(void);
	static void draw_opt_double(void);
	static void draw_opt_string(void);

	//operation node
	static void draw_op(void);
	static void draw_opt_input(void);
};
}

#endif //_CREATE_NODE_HPP
