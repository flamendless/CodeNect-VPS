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

	NODE_SLOT in_slot_si = NODE_SLOT::INTEGER;
	NODE_SLOT in_slot_ei = NODE_SLOT::INTEGER;
	NODE_SLOT in_slot_inc = NODE_SLOT::INTEGER;
	NODE_SLOT out_slot_it = NODE_SLOT::INTEGER;

	m_in_slots.push_back({"INTEGER - start index", in_slot_si});
	m_in_slots.push_back({"INTEGER - end index", in_slot_ei});
	m_in_slots.push_back({"INTEGER - increment", in_slot_inc});
	m_out_slots.push_back({"INTEGER - iterator", out_slot_it});

	PLOGD << "Created NodeLoop: " << m_name;
}

void NodeFor::create_str_code(void)
{
	int inc = m_cur_increment;
	int si = m_cur_start_index;
	int se = m_cur_end_index;

	const char* cmp_op = Utils::cmp_to_op_str(m_cmp);
	const char* sign = (inc > 0) ? "+" : "-";
	std::string str_inc;

	if (inc == 1)
		str_inc = fmt::format("{:s}++", m_iterator_name);
	else if (inc == -1)
		str_inc = fmt::format("{:s}--", m_iterator_name);
	else
		str_inc = fmt::format("{:s} {:s} {:d}", m_iterator_name, sign, inc);

	m_code = fmt::format("for (int {:s} = {:d}; {:s} {:s} {:d}; {:s})",
		m_iterator_name, si, m_iterator_name, cmp_op, se, str_inc);
}
}
