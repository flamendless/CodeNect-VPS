#ifndef _TEMP_DATA_HPP
#define _TEMP_DATA_HPP

#include "modules/node.hpp"
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
}

#endif //_TEMP_DATA_HPP
