#ifndef _NODE_CMP_HPP
#define _NODE_CMP_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeComparison : public Node
{
	static std::map<std::string, const char*> m_cmp_str;

	NODE_KIND m_kind = NODE_KIND::COMPARISON;
	NODE_CMP m_cmp = NODE_CMP::EMPTY;

	bool m_has_valid_connections = false;
	std::string m_str;
	bool current_res = false;

	explicit NodeComparison(
		NODE_CMP cmp,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeComparison() {}
	const char* get_cmp_op(void);
};
}

#endif //_NODE_CMP_HPP
