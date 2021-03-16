#ifndef _CREATE_NODE_HPP
#define _CREATE_NODE_HPP

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

struct TempVarData
{
	NODE_SLOT slot = NODE_SLOT::EMPTY;
	char buf_name[BUF_SIZE] = "";
	char buf_string[BUF_SIZE * 2] = "";
	bool valid_name = false;
	bool valid_value = false;
	int res_name = RES_VARNAME_INVALID;
	NodeValue value;

	int temp_int = 0;
	float temp_float = 0;
	double temp_double = 0;
};

struct TempOperationData
{
	std::vector<NODE_SLOT> v_slots_in;
	std::vector<NODE_SLOT> v_slots_out;
	NODE_OP op = NODE_OP::EMPTY;
	NodeValue value;
	bool valid_op = false;
};

struct CreateNode
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static bool is_pos_locked;
	static const char* title;
	static NODE_KIND kind;
	static bool can_create;

	union data
	{
		static TempVarData* var;
		static TempOperationData* op;
	};

	CreateNode() = delete;
	static void open(NODE_KIND kind);
	static void close(void);
	static void create_var_node(void);
	static void create_op_node(void);

	static void draw(void);
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
	static void draw_slots(void);
	static void draw_opt_input(void);
};
}

#endif //_CREATE_NODE_HPP
