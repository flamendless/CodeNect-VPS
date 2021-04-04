#ifndef _TEMP_DATA_HPP
#define _TEMP_DATA_HPP

#include "node/node.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
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
	NODE_OP op = NODE_OP::EMPTY;
	NODE_SLOT slot = NODE_SLOT::EMPTY;
	bool valid_op = false;
};

struct TempCastData
{
	NODE_SLOT slot_in = NODE_SLOT::EMPTY;
	NODE_SLOT slot_out = NODE_SLOT::EMPTY;
	bool valid_cast = false;
};

struct TempComparisonData
{
	NODE_SLOT slot_in = NODE_SLOT::EMPTY;
	NODE_CMP cmp = NODE_CMP::EMPTY;
	bool valid_cmp = false;
	std::string str = "EMPTY";
};

struct TempBranchData
{
	bool valid_branch = true;
};

struct TempActionData
{
	//print
	char buf_str[BUF_SIZE * 2] = "";
	bool is_override = false;
	bool is_append_newline = false;

	NODE_SLOT slot_in = NODE_SLOT::EMPTY;
	NODE_SLOT slot_out = NODE_SLOT::EMPTY;

	bool valid_action = false;
};

struct TempMathData
{
	NODE_SLOT slot_in_first = NODE_SLOT::EMPTY;
	NODE_SLOT slot_in_second = NODE_SLOT::EMPTY;
	NODE_SLOT slot_out = NODE_SLOT::EMPTY;
	bool valid_math = false;
};
}

#endif //_TEMP_DATA_HPP
