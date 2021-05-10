#ifndef _NODE_FOR_HPP
#define _NODE_FOR_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"
#include "node/node_loop.hpp"

namespace CodeNect
{
struct NodeFor : public NodeLoop
{
	NODE_LOOP m_loop = NODE_LOOP::FOR;

	std::string m_code;
	std::string m_iterator_name = "i";
	int m_start_index = 0;
	int m_end_index = 0;
	int m_increment = 0;
	NODE_CMP m_cmp = NODE_CMP::EMPTY;

	int m_cur_start_index = 0;
	int m_cur_end_index = 0;
	int m_cur_increment = 0;

	int m_cur_it = 0;
	int m_override_it = 0;
	bool m_has_reached_end = false;

	explicit NodeFor(
		NODE_CMP cmp,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);
	inline ~NodeFor() {}
	void create_str_code(void);
};
}

#endif //_NODE_FOR_HPP
