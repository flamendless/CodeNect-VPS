#ifndef _NODE_PRINT_HPP
#define _NODE_PRINT_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodePrint : public Node
{
	NODE_KIND m_kind = NODE_KIND::PRINT;
	std::string m_string;
	bool m_append_newline = false;

	explicit NodePrint(
		std::string str,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodePrint() {}
};
}

#endif //_NODE_PRINT_HPP
