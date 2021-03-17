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

//NOTE:
//"in node" is the current node
//"out node" is the previous node
void NodeVariable::on_connect(Node* in, Node* out)
{
	NODE_KIND* out_kind = &out->m_kind;

	switch (*out_kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE: change_value((NodeVariable*)out); break;
		case NODE_KIND::OPERATION: break;
		case NODE_KIND::IF: break;
	}
}

void NodeVariable::change_value(NodeVariable* out)
{
	m_value = out->m_value;

	std::string msg = fmt::format("{}'s' value of {} is set to {}'s {}",
		m_name, m_value.get_value_str(), out->m_name, out->m_value.get_value_str());
	PLOGD << msg;
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
