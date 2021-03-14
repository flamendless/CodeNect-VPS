#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include "ImNodesEz.h"
#include "enum.h"
#include "modules/nodes_connection.hpp"

namespace CodeNect
{
BETTER_ENUM(NODE_SLOT, char, EMPTY = 1, BOOL, INT, FLOAT, DOUBLE, STRING)
BETTER_ENUM(NODE_KIND, char, EMPTY = 1, VARIABLE)

struct Node
{
	typedef std::vector<Connection> v_connection_t;
	typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;

	NODE_KIND m_kind = NODE_KIND::EMPTY;
	const char* m_str_kind;
	const char* m_name;
	const char* m_value;
	bool m_selected = false;
	ImVec2 m_pos{};
	v_connection_t m_connections{};
	v_slot_info_t m_in_slots{};
	v_slot_info_t m_out_slots{};

	explicit Node(
		NODE_KIND kind,
		const char* name,
		const char* value,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	)
		: m_kind{kind}, m_name{name}, m_value{value}, m_in_slots{in_slots}, m_out_slots{out_slots}
	{
		m_str_kind = kind._to_string();
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
