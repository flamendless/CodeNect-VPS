#include "modules/node_var.hpp"

namespace CodeNect
{
NodeVariable::NodeVariable(
	const char* name,
	NodeValue value,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	Node::m_kind = m_kind;
	Node::m_name = name;
	m_value = value;
	m_value_orig = value;
	Node::m_in_slots = in_slots;
	Node::m_out_slots = out_slots;

	PLOGD << "Created NodeVariable: " << m_name << ", value = " << m_value.get_value_str();
}

void NodeVariable::draw(void)
{
	if (ImGui::BeginTable("TableNode##NodeVar", 2, ImGuiTableFlags_SizingFixedFit))
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
}
