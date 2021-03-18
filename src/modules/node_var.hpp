#ifndef _NODE_VAR_HPP
#define _NODE_VAR_HPP

#include "fmt/format.h"
#include "plog/Log.h"
#include "modules/node_def.hpp"
#include "modules/node.hpp"
#include "modules/node_val.hpp"

namespace CodeNect
{
struct NodeVariable : public Node
{
	NodeValue* m_value;
	NodeValue m_value_orig;
	NODE_KIND m_kind = NODE_KIND::VARIABLE;

	explicit NodeVariable(
		const char* name,
		NodeValue value,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeVariable() {}
	void draw(void) override;
};
}

#endif //_NODE_VAR_HPP
