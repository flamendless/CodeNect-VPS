#ifndef _NODE_HPP
#define _NODE_HPP

#include "ImNodesEz.h"
#include "plog/Log.h"
#include "modules/node_def.hpp"
#include "modules/node_val.hpp"
#include "modules/connection.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
struct Node
{
	bool m_selected = false;
	bool m_to_delete = false;
	ImVec2 m_pos{};

	NODE_KIND m_kind = NODE_KIND::EMPTY;
	const char* m_name;
	const char* m_desc = "";

	v_connection_t m_connections{};
	v_slot_info_t m_in_slots{};
	v_slot_info_t m_out_slots{};

	inline Node() {}
	inline virtual ~Node() {}

	void delete_connection(const Connection& connection);
};
}

#endif //_NODE_HPP
