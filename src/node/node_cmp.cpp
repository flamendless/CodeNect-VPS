#include "node/node_cmp.hpp"

#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
std::map<std::string, const char*> NodeComparison::m_cmp_str
{
	{"EQ", "equal " ICON_FA_EQUALS},
	{"NEQ", "not equal " ICON_FA_NOT_EQUAL},
	{"LT", "less than " ICON_FA_LESS_THAN},
	{"GT", "greater than " ICON_FA_GREATER_THAN},
	{"LTE", "less than or equals to " ICON_FA_LESS_THAN_EQUAL},
	{"GTE", "greater than or equals to " ICON_FA_GREATER_THAN_EQUAL},
	{"OR", "or" ICON_FA_GRIP_LINES_VERTICAL},
	{"AND", "and" "  &"},
};

NodeComparison::NodeComparison(
	NODE_CMP cmp,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	PPK_ASSERT(cmp != +NODE_CMP::EMPTY, "Passsed NODE_CMP should not be EMPTY");

	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(m_kind._to_string());
	m_cmp = cmp;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	PLOGD << "Created NodeComparison: " << m_name << ", " << m_cmp._to_string();
}

const char* NodeComparison::get_cmp_op(void)
{
	switch (m_cmp)
	{
		case NODE_CMP::EMPTY: break;
		case NODE_CMP::EQ: return "=="; break;
		case NODE_CMP::NEQ: return "!="; break;
		case NODE_CMP::LT: return "<"; break;
		case NODE_CMP::GT: return ">"; break;
		case NODE_CMP::LTE: return "<="; break;
		case NODE_CMP::GTE: return ">="; break;
		case NODE_CMP::OR: return "||"; break;
		case NODE_CMP::AND: return "&&"; break;
	}

	return "";
}
}
