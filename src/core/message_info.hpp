#ifndef _MESSAGE_INFO_HPP
#define _MESSAGE_INFO_HPP

#include <string>
#include "node/node.hpp"

namespace CodeNect
{
enum class OUTPUT_TYPE { NORMAL = 1, SUCCESS, ERROR, WARNING, PROMPT };

struct MessageInfo
{
	OUTPUT_TYPE m_type = OUTPUT_TYPE::NORMAL;
	std::string m_msg;
	Node* m_node = nullptr;
	//TODO add here uid for docu message, e.g DOC_ID::BRANCH_CONFLICT
};
}

#endif //_MESSAGE_INFO_HPP
