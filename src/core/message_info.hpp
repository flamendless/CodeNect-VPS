#ifndef _MESSAGE_INFO_HPP
#define _MESSAGE_INFO_HPP

#include <string>
#include "node/node.hpp"
#include "enums.h"

namespace CodeNect
{
enum class OUTPUT_TYPE { NORMAL = 1, SUCCESS, ERROR, WARNING, PROMPT };
BETTER_ENUM(DOC_ID, int, EMPTY = 1, BRANCH_CONFLICT, AOOB, VAR_SIZE, NEED_INPUTS,
		CMP_REQ, MATH_REQ, OP_REQ)

struct MessageInfo
{
	OUTPUT_TYPE m_type = OUTPUT_TYPE::NORMAL;
	std::string m_msg;
	Node* m_node = nullptr;
	DOC_ID m_doc_id = DOC_ID::EMPTY;
};
}

#endif //_MESSAGE_INFO_HPP