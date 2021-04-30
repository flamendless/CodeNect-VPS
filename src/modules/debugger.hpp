#ifndef _DEBUGGER_HPP
#define _DEBUGGER_HPP

#include "node/node.hpp"

namespace CodeNect
{
struct Debugger
{
	static std::map<int, Node*> m_nodes;

	static void jump_to_node_at_index(int);
};
}

#endif //_DEBUGGER_HPP
