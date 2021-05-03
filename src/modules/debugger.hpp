#ifndef _DEBUGGER_HPP
#define _DEBUGGER_HPP

#include "node/node.hpp"
#include "core/message_info.hpp"

namespace CodeNect
{
struct Debugger
{
	static std::vector<MessageInfo> v_msg_info;

	static void add_message(const std::string&, OUTPUT_TYPE = OUTPUT_TYPE::WARNING, Node* = nullptr);
	static void clear(void);
};
}

#endif //_DEBUGGER_HPP
