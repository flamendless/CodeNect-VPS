#ifndef _NODE_SET_HPP
#define _NODE_SET_HPP

#include "node/node_action.hpp"
#include "node/node_var.hpp"

namespace CodeNect
{
struct NodeSet : public NodeAction
{
	NODE_ACTION m_action = NODE_ACTION::SET;

	NodeVariable* m_node_var;
	NodeValue m_node_val;
	NodeValue m_node_val_orig;

	explicit NodeSet(
		NodeVariable* node_var,
		NodeValue& node_val,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeSet() {}
};
}

#endif //_NODE_SET_HPP
