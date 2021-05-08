#include "node/node_for.hpp"

#include "plog/Log.h"
#include "ppk_assert.h"
#include "fmt/format.h"
#include "node/nodes.hpp"
#include "core/utils.hpp"

namespace CodeNect
{
NodeFor::NodeFor(
	NODE_CMP cmp,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	NodeLoop::m_loop = m_loop;
	Node::m_kind = NodeLoop::m_kind;
	Node::m_name = Nodes::get_id(m_loop._to_string());

	PPK_ASSERT(cmp != +NODE_CMP::EMPTY, "NODE_CMP must not be EMPTY");

	m_cmp = cmp;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	const char* cmp_op = Utils::cmp_to_op_str(m_cmp);
	const char* sign = (m_increment > 0) ? "+" : "-";

	m_code = fmt::format("for (int {:s} = {:d}; {:s} {:s} {:d}; {:s} {:s} {:s})",
		m_iterator_name, m_start_index, m_start_index, cmp_op, m_end_index,
		m_iterator_name, sign, m_increment);

	PLOGD << "Created NodeLoop: " << m_name;
}
}
