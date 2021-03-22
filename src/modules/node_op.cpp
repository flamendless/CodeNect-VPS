#include "modules/node_op.hpp"

#include "IconsFontAwesome5.h"
#include "core/utils.hpp"
#include "core/config.hpp"
#include "modules/nodes.hpp"

namespace CodeNect
{
NodeOperation::NodeOperation(
	NODE_OP op,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	Nodes::op_id++;

	Node::m_kind = m_kind;

	std::string str_op = op._to_string();
	std::string* str_id = new std::string(str_op + "_" + std::to_string(Nodes::op_id));

	Node::m_name = str_id->c_str();
	m_op = op;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	switch (m_op)
	{
		case NODE_OP::EMPTY: break;
		case NODE_OP::ADD: m_icon = ICON_FA_PLUS; break;
		case NODE_OP::SUB: m_icon = ICON_FA_MINUS; break;
		case NODE_OP::MUL: m_icon = ICON_FA_TIMES; break;
		case NODE_OP::DIV: m_icon = ICON_FA_DIVIDE; break;
	}

	PLOGD << "Created NodeOperation: " << m_name << ", op = " << m_op._to_string();
}

void NodeOperation::draw(void)
{
	if (ImGui::BeginTable("TableNode##NodeOperation", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Op:");
		ImGui::TextColored(Config::NodeInterface_c::label_color, "Desc:");

		ImGui::TableNextColumn();
		ImGui::Text("%s %s", m_op._to_string(), m_icon);
		ImGui::Text("%s", m_desc);

		ImGui::EndTable();
	}
}
}
