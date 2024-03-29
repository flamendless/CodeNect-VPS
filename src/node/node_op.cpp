#include "node/node_op.hpp"

#include "ppk_assert.h"
#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "core/utils.hpp"
#include "core/config.hpp"
#include "node/nodes.hpp"

namespace CodeNect
{
NodeOperation::NodeOperation(
	NODE_OP op,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	PPK_ASSERT(op != +NODE_OP::EMPTY, "Passsed NODE_OP should not be EMPTY");

	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(op._to_string());
	m_op = op;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	switch (m_op)
	{
		case NODE_OP::EMPTY: break;
		case NODE_OP::ADD: m_icon = ICON_FA_PLUS; break;
		case NODE_OP::SUB: m_icon = ICON_FA_MINUS; break;
		case NODE_OP::MUL: m_icon = ICON_FA_TIMES; break;
		case NODE_OP::DIV: m_icon = ICON_FA_SLASH; break;
		case NODE_OP::MOD: m_icon = ICON_FA_DIVIDE; break;
	}

	PLOGD << "Created NodeOperation: " << m_name << ", op = " << m_op._to_string();
}

const char* NodeOperation::get_op(void)
{
	switch (m_op)
	{
		case NODE_OP::EMPTY: break;
		case NODE_OP::ADD: return "+"; break;
		case NODE_OP::SUB: return "-"; break;
		case NODE_OP::MUL: return "*"; break;
		case NODE_OP::DIV: return "/"; break;
		case NODE_OP::MOD: return "%"; break;
	}

	PPK_ASSERT(false, "This should not be reached");

	return "";
}
}
