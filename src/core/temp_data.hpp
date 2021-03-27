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
	char buf_desc[BUF_SIZE * 2] = "";

	int temp_int = 0;
	float temp_float = 0;
	double temp_double = 0;
};

struct TempOperationData
{
	NODE_OP op = NODE_OP::EMPTY;
	NODE_SLOT slot = NODE_SLOT::EMPTY;
	char buf_desc[BUF_SIZE * 2] = "";
	bool valid_op = false;
};

struct TempCastData
{
	NODE_SLOT slot_in = NODE_SLOT::EMPTY;
	NODE_SLOT slot_out = NODE_SLOT::EMPTY;
	char buf_desc[BUF_SIZE * 2] = "";
	bool valid_cast = false;
};

struct TempComparisonData
{
	NODE_SLOT slot_in = NODE_SLOT::EMPTY;
	NODE_CMP cmp = NODE_CMP::EMPTY;
	char buf_desc[BUF_SIZE * 2] = "";
	bool valid_cmp = false;
	std::string str = "EMPTY";
};

struct TempBranchData
{
	char buf_desc[BUF_SIZE * 2] = "";
	bool valid_branch = false;
};
}

#endif //_TEMP_DATA_HPP
