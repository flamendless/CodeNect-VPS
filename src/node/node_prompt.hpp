#ifndef _NODE_PROMPT_HPP
#define _NODE_PROMPT_HPP

#include "node/node_action.hpp"

namespace CodeNect
{
struct NodePrompt : public NodeAction
{
	NODE_ACTION m_action = NODE_ACTION::PROMPT;

	std::string m_orig_str = "";
	std::string m_str;
	bool m_override = false;
	bool m_append_newline = false;
	NodeValue m_fake_input;

	explicit NodePrompt(
		std::string str,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodePrompt() {}
};
}

#endif //_NODE_PROMPT_HPP
