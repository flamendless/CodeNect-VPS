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
	NodeValue m_value;
	NODE_KIND m_kind = NODE_KIND::VARIABLE;

	explicit NodeVariable(
		const char* name,
		NodeValue value,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeVariable() {}

	//NOTE:
	//"in node" is the current node
	//"out node" is the previous node
	void change_value(NodeVariable* out);
	void on_connect(Node* in, Node* out) override;
	void draw(void) override;
};
}

#endif //_NODE_VAR_HPP
