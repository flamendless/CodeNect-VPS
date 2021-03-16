#ifndef _NODES_HPP
#define _NODES_HPP

#include <vector>
#include <map>
#include <string>
#include "modules/nodes_connection.hpp"
#include "ImNodesEz.h"
#include "modules/node.hpp"

namespace CodeNect
{
struct Nodes
{
	typedef std::map<std::string, Node*(*)()> m_node_t;

	static std::vector<Node*> v_nodes;
	static m_node_t m_available_nodes;

	Nodes() = delete;
};
};

#endif //_NODES_HPP
