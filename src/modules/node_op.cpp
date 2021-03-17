#include "modules/node_op.hpp"

#include "IconsFontAwesome5.h"
#include "core/utils.hpp"

namespace CodeNect
{
NodeOp::NodeOp(
	NODE_OP op,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	Node::m_kind = m_kind;
	Node::m_name = op._to_string();
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
}

void NodeOp::on_connect(Node* in, Node* out)
{
}

void NodeOp::draw(void)
{
	if (ImGui::BeginTable("TableNode##NodeOp", 2, ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Operation:");

		ImGui::TableNextColumn();
		ImGui::Text("%s %s", m_op._to_string(), m_icon);

		ImGui::EndTable();
	}
}
}
