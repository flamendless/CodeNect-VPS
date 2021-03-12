#ifndef NODES_HPP
#define NODES_HPP

#include <vector>
#include <map>
#include <string>
#include "modules/nodes_connection.hpp"
#include "imgui.h"
#include "ImNodesEz.h"

namespace CodeNect
{
enum NODE_SLOT
{
	BOOL = 1,
	INT,
	FLOAT,
	DOUBLE,
	STRING,
};

struct Node
{
	typedef std::vector<Connection> v_connection_t;
	typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;

	const char* m_title;
	bool m_selected = false;
	ImVec2 m_pos{};
	v_connection_t m_connections{};
	v_slot_info_t m_in_slots{};
	v_slot_info_t m_out_slots{};

	explicit Node(const char* title,
		const v_slot_info_t&& input_slots,
		const v_slot_info_t&& output_slots)
	{
		m_title = title;
		m_in_slots = input_slots;
		m_out_slots = output_slots;
	}

	void delete_connection(const Connection& connection)
	{
		for (v_connection_t::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
		{
			if (connection == *it)
			{
				m_connections.erase(it);
				break;
			}
		}
	}
};

struct Nodes
{
	typedef std::map<std::string, Node*(*)()> m_node_t;

	static std::vector<Node*> v_nodes;
	static m_node_t m_available_nodes;

	Nodes() = delete;
};
};

#endif //NODES_HPP
