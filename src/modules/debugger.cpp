#include "modules/debugger.hpp"

#include "plog/Log.h"
#include "ui/node_interface.hpp"

namespace CodeNect
{
std::map<int, Node*> Debugger::m_nodes;

void Debugger::jump_to_node_at_index(int index)
{
	bool exists = Debugger::m_nodes.find(index) != Debugger::m_nodes.end();
	if (exists)
	{
		Node* node = Debugger::m_nodes.at(index);
		NodeInterface::jump_to_pos(node);
	}
	else
		PLOGE << "index " << index << " is not found.";
}
}
