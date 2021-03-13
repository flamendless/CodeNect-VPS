#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include "ImNodesEz.h"
#include "enum.h"
#include "modules/nodes_connection.hpp"

namespace CodeNect
{
BETTER_ENUM(NODE_SLOT, char, EMPTY = 1, BOOL, INT, FLOAT, DOUBLE, STRING)

struct Node
{
	typedef std::vector<Connection> v_connection_t;
	typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;

	const char* m_var_name;
	bool m_selected = false;
	ImVec2 m_pos{};
	v_connection_t m_connections{};
	v_slot_info_t m_in_slots{};
	v_slot_info_t m_out_slots{};

	explicit Node(const char* var_name,
		const v_slot_info_t&& input_slots,
		const v_slot_info_t&& output_slots)
	{
		m_var_name = var_name;
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
}

#endif //NODE_HPP
