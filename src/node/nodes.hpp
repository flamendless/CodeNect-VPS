#ifndef _NODES_HPP
#define _NODES_HPP

#include <vector>
#include <map>
#include <string>
#include "ImNodesEz.h"
#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_meta.hpp"
#include "node/node_entry.hpp"

namespace CodeNect
{
struct Nodes
{
	typedef std::map<std::string, Node*(*)()> m_node_t;

	static std::vector<Node*> v_nodes;
	static std::map<std::string, unsigned int> m_ids;
	static m_node_t m_available_nodes;
	static std::map<std::string, const char*> m_names;
	static bool has_entry;

	Nodes() = delete;
	static void reset(void);
	static const char* get_id(const char* id);
	static void delete_node(std::vector<Node*>::iterator& it);
	static Node* find_by_name(const char* name);
	static NodeEntry* find_node_entry(void);
	static void build_slots(NodeMeta& meta, v_slot_info_t& in, v_slot_info_t& out);
	static void build_from_meta(const std::vector<NodeMeta*> &v_node_meta);
	static void build_from_meta(const std::vector<ConnectionMeta*> &v_connection_meta);
	static unsigned int count_connections(void);
	static const char* get_title(Node* node);
};
}

#endif //_NODES_HPP
