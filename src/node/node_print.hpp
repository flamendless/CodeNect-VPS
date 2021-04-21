#ifndef _NODE_PRINT_HPP
#define _NODE_PRINT_HPP

#include "node/node_action.hpp"

namespace CodeNect
{
struct NodePrint : public NodeAction
{
	NODE_ACTION m_action = NODE_ACTION::PRINT;

	std::string m_orig_str = "";
	std::string m_str;
	bool m_append = false;
	bool m_append_newline = false;
	bool m_override = false;

	explicit NodePrint(
		std::string str,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodePrint() {}
};
}

#endif //_NODE_PRINT_HPP
