#ifndef _NODES_HPP
#define _NODES_HPP

#include <vector>
#include <map>
#include <string>
#include "modules/connection.hpp"
#include "ImNodesEz.h"
#include "modules/node.hpp"
#include "modules/node_meta.hpp"

namespace CodeNect
{
struct Nodes
{
	typedef std::map<std::string, Node*(*)()> m_node_t;

	static bool has_built_meta;
	static std::vector<Node*> v_nodes;
	static m_node_t m_available_nodes;

	Nodes() = delete;
	static void build_slots(NodeMeta& meta, v_slot_info_t& in, v_slot_info_t& out);
	static void build_from_meta(const std::vector<NodeMeta*> &meta);
};
};

#endif //_NODES_HPP
