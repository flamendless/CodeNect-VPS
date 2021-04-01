#ifndef _NODE_MATH_HPP
#define _NODE_MATH_HPP

#include <variant>
#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct MathRoot
{
	bool m_has_connections = false;
	int m_index = 0;
	std::variant<int, float, double> m_radicand = 0;
};

struct NodeMath : public Node
{
	NODE_KIND m_kind = NODE_KIND::MATH;
	NODE_MATH m_math = NODE_MATH::EMPTY;

	std::variant<MathRoot> m_data;

	explicit NodeMath(
		NODE_MATH math,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeMath() {}
};
}

#endif //_NODE_MATH_HPP
