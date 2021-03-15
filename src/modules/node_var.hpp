#ifndef NODE_VAR_HPP
#define NODE_VAR_HPP

#include "modules/node_def.hpp"
#include "modules/node.hpp"

namespace CodeNect
{
struct NodeVariable : public Node
{
	NodeValue m_value;

	inline explicit NodeVariable(
		NODE_KIND kind,
		const char* name,
		NodeValue value,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	)
	{
		m_kind = kind;
		m_str_kind = kind._to_string();
		m_name = name;
		m_value = value;
		m_in_slots = in_slots;
		m_out_slots = out_slots;
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

#endif //NODE_VAR_HPP
