#ifndef _NODES_HPP
#define _NODES_HPP

#include <vector>
#include <map>
#include <string>
#include "ImNodesEz.h"
#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_meta.hpp"

namespace CodeNect
{
struct Nodes
{
	typedef std::map<std::string, Node*(*)()> m_node_t;

	static std::vector<Node*> v_nodes;
	static std::map<std::string, unsigned int> m_ids;
	static m_node_t m_available_nodes;
	static std::map<std::string, const char*> m_names;

	Nodes() = delete;
	static void reset(void);
	static const char* get_id(const char* id);
	static void delete_node(std::vector<Node*>::iterator& it);
	static bool check_if_no_lhs(Node* node);
	static Node* find_by_name(const char* name);
	static Node* find_connected_by_value(Node* node, NodeValue* val);
	static void build_slots(NodeMeta& meta, v_slot_info_t& in, v_slot_info_t& out);
	static void build_from_meta(const std::vector<NodeMeta*> &v_node_meta);
	static void build_from_meta(const std::vector<ConnectionMeta*> &v_connection_meta);
	static unsigned int count_connections(void);
	static const char* get_title(Node* node);
};
}

#endif //_NODES_HPP
