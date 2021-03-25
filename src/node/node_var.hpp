#ifndef _NODE_VAR_HPP
#define _NODE_VAR_HPP

#include "fmt/format.h"
#include "plog/Log.h"
#include "node/node_def.hpp"
#include "node/node.hpp"
#include "node/node_val.hpp"

namespace CodeNect
{
struct NodeVariable : public Node
{
	NODE_KIND m_kind = NODE_KIND::VARIABLE;
	NodeValue m_value_orig;
	NodeValue m_value;

	explicit NodeVariable(
		const char* name,
		NodeValue value,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeVariable() {}
};
}

#endif //_NODE_VAR_HPP
