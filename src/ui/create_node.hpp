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
	static const char* edit_title;

	static NODE_KIND kind;
	static NODE_ACTION action;
	static NODE_MATH math;
	static NODE_DS ds;
	static NODE_GET get;
	static NODE_STRING str;
	static NODE_LOOP loop;

	static Node* node_to_edit;
	static bool can_create;
	static std::variant<
			TempVarData*, TempOperationData*,
			TempCastData*, TempComparisonData*,
			TempBranchData*, TempActionData*,
			TempMathData*, TempArrayData*,
			TempGetData*, TempStringData*,
			TempLoopData*
		>data;
	static char buf_desc[BUF_SIZE * 2];

	CreateNode() = delete;
	static void open(NODE_KIND kind);
	static void open_action(NODE_KIND, NODE_ACTION);
	static void open_math(NODE_KIND, NODE_MATH);
	static void open_ds(NODE_KIND, NODE_DS);
	static void open_get(NODE_KIND, NODE_GET);
	static void open_string(NODE_KIND, NODE_STRING);
	static void open_loop(NODE_KIND, NODE_LOOP);
	static void edit(Node* node);
	static void close(void);

	//creation
	static void create_node_var(void);
	static void create_node_op(void);
	static void create_node_cast(void);
	static void create_node_cmp(void);
	static void create_node_branch(void);
	static void create_node_print(void);
	static void create_node_prompt(void);
	static void create_node_math(void);
	static void create_node_array(void);
	static void create_node_array_access(void);
	static void create_node_size(void);
	static void create_node_string(void);
	static void create_node_for(void);
	static void create_node_set(void);

	static void draw(void);
	static void draw_desc(void);
	static void draw_buttons(void);

	//NodeVariable
	static void draw_var(void);
	static void draw_opt_bool(NodeValue*);
	static void draw_opt_int(NodeValue*, int&);
	static void draw_opt_float(NodeValue*, float&);
	static void draw_opt_double(NodeValue*, double&);
	static void draw_opt_string(NodeValue*, char[BUF_SIZE * 2]);

	//NodeOperation
	static void draw_op(void);
	static void draw_opt_input(void);

	//NodeCast
	static void draw_cast(void);

	//NodeComparison
	static void draw_cmp(void);

	//NodeAction
	static void draw_print(void);
	static void draw_prompt(void);
	static void draw_set(void);

	//NodeMath
	static void draw_math(void);
	static void draw_math_opt(const char* title, NODE_SLOT& slot);

	//NodeArray
	static void draw_array(void);
	static void draw_array_elements(void);

	//NodeGet
	static void draw_array_access(void);
	static void draw_size(void);

	//Loop
	static void draw_for(void);
};
}

#endif //_CREATE_NODE_HPP
