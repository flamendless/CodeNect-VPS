#include "modules/node_logic.hpp"

#include "plog/Log.h"

namespace CodeNect
{
namespace NodeLogic
{
void change_value(NodeVariable* in, NodeVariable* out)
{
	in->m_value = out->m_value;

	std::string msg = fmt::format("{}'s' value of {} is set to {}'s {}",
		in->m_name, in->m_value.get_value_str(), out->m_name, out->m_value.get_value_str());
	PLOGD << msg;
}

//NOTE:
//"in node" is the current node
//"out node" is the previous node
void on_connect(Node* in, Node* out)
{
	NODE_KIND* out_kind = &out->m_kind;

	switch (*out_kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE:NodeLogic::change_value((NodeVariable*)in, (NodeVariable*)out); break;
		case NODE_KIND::OPERATION: break;
		case NODE_KIND::IF: break;
	}
}
}
}
