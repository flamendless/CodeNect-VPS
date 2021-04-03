#ifndef _NODE_MATH_HPP
#define _NODE_MATH_HPP

#include <functional>
#include <map>
#include <variant>
#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeMath : public Node
{
	static std::map<std::string, const char*> m_tooltips;
	static std::map<std::string, std::function<double(double, double)>> m_functions;

	NODE_KIND m_kind = NODE_KIND::MATH;
	NODE_MATH m_math = NODE_MATH::EMPTY;

	bool m_has_connections = false;
	std::variant<int, float, double> m_first = 0; //index, base, sin, cos, tan
	std::variant<int, float, double> m_second = 0; //radicand, power

	explicit NodeMath(
		NODE_MATH math,
		const v_slot_info_t&& in_slots,
		const v_slot_info_t&& out_slots
	);

	inline ~NodeMath() {}
	void reset(void);
	const char* get_string_a(void);
	const char* get_string_b(void);
	const char* get_icon(void);
	void draw_first(void);
	void draw_second(void);
};
}

#endif //_NODE_MATH_HPP
