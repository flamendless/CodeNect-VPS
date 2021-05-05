#include "modules/debugger.hpp"

#include "plog/Log.h"
#include "ui/node_interface.hpp"

namespace CodeNect
{
std::vector<MessageInfo> Debugger::v_msg_info;

void Debugger::add_message(const std::string& msg, OUTPUT_TYPE type, Node* node, DOC_ID doc_id)
{
	MessageInfo info;
	info.m_msg = std::move(msg);
	info.m_type = type;
	info.m_node = node;
	info.m_doc_id = doc_id;
	Debugger::v_msg_info.push_back(std::move(info));
}

void Debugger::clear(void)
{
	Debugger::v_msg_info.clear();
}
}
