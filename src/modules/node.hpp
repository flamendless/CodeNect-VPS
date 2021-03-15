#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include "ImNodesEz.h"
#include "plog/Log.h"
#include "enum.h"
#include "modules/nodes_connection.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
BETTER_ENUM(NODE_SLOT, char, EMPTY = 1, BOOL, INTEGER, FLOAT, DOUBLE, STRING)
BETTER_ENUM(NODE_KIND, char, EMPTY = 1, VARIABLE)

struct NodeValue
{
	NODE_SLOT m_slot = NODE_SLOT::EMPTY;

	union
	{
		bool v_bool;
		int v_int;
		float v_float;
		double v_double;
		const char* v_string;
	};

	void set(bool arg){ v_bool = arg; m_slot = NODE_SLOT::BOOL; }
	void set(int arg){ v_int = arg; m_slot = NODE_SLOT::INTEGER; }
	void set(float arg){ v_float = arg; m_slot = NODE_SLOT::FLOAT; }
	void set(double arg){ v_double = arg; m_slot = NODE_SLOT::DOUBLE; }
	void set(const char* arg){ v_string = arg; m_slot = NODE_SLOT::STRING; }

	void* get_value()
	{
		// void* d = node.m_value.get_value();
		// const char* str = static_cast<const char*>(d);
		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: return nullptr; break;
			case NODE_SLOT::BOOL: return &v_bool; break;
			case NODE_SLOT::INTEGER: return &v_int; break;
			case NODE_SLOT::FLOAT: return &v_float; break;
			case NODE_SLOT::DOUBLE: return &v_double; break;
			case NODE_SLOT::STRING: return (void*)v_string; break;
		}
	}

	const char* get_spec()
	{
		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: return nullptr; break;
			case NODE_SLOT::BOOL: return "%d"; break;
			case NODE_SLOT::INTEGER: return "%d"; break;
			case NODE_SLOT::FLOAT: return "%f"; break;
			case NODE_SLOT::DOUBLE: return "%lf"; break;
			case NODE_SLOT::STRING: return "%s"; break;
		}
	}

	void draw()
	{
		switch (m_slot)
		{
			case NODE_SLOT::EMPTY: ImGui::Text("Empty?! (this should be an error)"); break;
			case NODE_SLOT::BOOL: ImGui::Text("%s", v_bool ? "true" : "false"); break;
			case NODE_SLOT::INTEGER: ImGui::Text("%d", v_int); break;
			case NODE_SLOT::FLOAT: ImGui::Text("%f", v_float); break;
			case NODE_SLOT::DOUBLE: ImGui::Text("%.8lf", v_double); break;
			case NODE_SLOT::STRING: ImGui::Text("%s", v_string); break;
		}
	}
};

struct Node
{
	typedef std::vector<Connection> v_connection_t;
	typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;

	bool m_selected = false;
	ImVec2 m_pos{};

	NODE_KIND m_kind = NODE_KIND::EMPTY;
	const char* m_str_kind;
	const char* m_name;
	NodeValue m_value;
	v_connection_t m_connections{};
	v_slot_info_t m_in_slots{};
	v_slot_info_t m_out_slots{};

	explicit Node(
		NODE_KIND kind,
		const char* name,
		NodeValue value,
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
