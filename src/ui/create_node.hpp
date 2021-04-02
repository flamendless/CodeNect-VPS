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
	static NODE_ACTION action;
	static NODE_MATH math;

	static Node* node_to_edit;
	static bool can_create;
	static std::variant<
			TempVarData*, TempOperationData*,
			TempCastData*, TempComparisonData*,
			TempBranchData*, TempActionData*,
			TempMathData*
		>data;
	static char buf_desc[BUF_SIZE * 2];

	CreateNode() = delete;
	static void open(NODE_KIND kind);
	static void open_action(NODE_KIND kind, NODE_ACTION action);
	static void open_math(NODE_KIND kind, NODE_MATH math);
	static void edit(Node* node);
	static void close(void);

	//creation
	static void create_node_var(void);
	static void create_node_op(void);
	static void create_node_cast(void);
	static void create_node_cmp(void);
	static void create_node_branch(void);
	static void create_node_print(void);
	static void create_node_math(void);

	static void draw(void);
	static void draw_desc(void);
	static void draw_buttons(void);

	//NodeVariable
	static void draw_var(void);
	static void draw_opt_bool(void);
	static void draw_opt_int(void);
	static void draw_opt_float(void);
	static void draw_opt_double(void);
	static void draw_opt_string(void);

	//NodeOperation
	static void draw_op(void);
	static void draw_opt_input(void);

	//NodeCast
	static void draw_cast(void);

	//NodeComparison
	static void draw_cmp(void);

	//NodePrint
	static void draw_print(void);

	//NodeMath
	static void draw_math(void);
	static void draw_math_opt(const char* title, NODE_SLOT& slot);
};
}

#endif //_CREATE_NODE_HPP
