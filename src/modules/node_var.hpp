#ifndef _NODE_VAR_HPP
#define _NODE_VAR_HPP

#include "plog/Log.h"
#include "modules/node_def.hpp"
#include "modules/node.hpp"
#include "modules/node_val.hpp"

namespace CodeNect
{
struct NodeVariable : public Node
{
	NodeValue m_value;
	NODE_KIND m_kind = NODE_KIND::VARIABLE;

	inline explicit NodeVariable(
		const char* name,
		NodeValue value,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	)
	{
		m_str_kind = m_kind._to_string();
		m_name = name;
		m_value = value;
		m_in_slots = in_slots;
		m_out_slots = out_slots;

		PLOGD << "Created NodeVariable: " << m_name << ", " << m_value.get_value();
	}

	inline ~NodeVariable() {}

	inline void draw_node(void) override
	{
		if (ImGui::BeginTable("TableNode", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Name:");
			ImGui::Text("Value:");

			ImGui::TableNextColumn();
			ImGui::Text("%s", m_name);
			m_value.draw();

			ImGui::EndTable();
		}
	}
};
}

#endif //_NODE_VAR_HPP
