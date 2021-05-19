#ifndef _MESSAGE_INFO_HPP
#define _MESSAGE_INFO_HPP

#include <string>
#include "node/node.hpp"
#include "enum.h"

namespace CodeNect
{
enum class OUTPUT_TYPE { NORMAL = 1, SUCCESS, ERR, WARNING, STDOUT };
BETTER_ENUM(DOC_ID, int, EMPTY = 1,
		//for transpiler/node_logic warnings/errors
		BRANCH_CONFLICT, AOOB, VAR_SIZE, NEED_INPUTS, CMP_REQ, MATH_REQ, OP_REQ,
		BRANCH_REQ, FOR_LOOP,

		//for connection warning/errors
		ARRAY_TO_PRINT, CANT_REF_ARRAY, IT_MUST_BE_VAR, CYCLIC,

		//others
		HOW_TO_SUBMIT_ASSESSMENT
	)

struct MessageInfo
{
	OUTPUT_TYPE m_type = OUTPUT_TYPE::NORMAL;
	std::string m_msg;
	Node* m_node = nullptr;
	DOC_ID m_doc_id = DOC_ID::EMPTY;
};
}

#endif //_MESSAGE_INFO_HPP
