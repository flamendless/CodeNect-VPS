#include "node/node_math.hpp"

#include <cmath>
#include "ppk_assert.h"
#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "plog/Log.h"
#include "node/nodes.hpp"

namespace CodeNect
{
std::map<std::string, const char*> NodeMath::m_tooltips
{
	{"ROOT", "get the n-th (index) root of the radicand"},
	{"POW", "get the result of raising the base number by the power of exponent"},
	{"SIN", "get the sine of the number"},
	{"COS", "get the cosine of the number"},
	{"TAN", "get the tangent of the number"},
};

std::map<std::string, std::function<double(double, double)>> NodeMath::m_functions
{
	{"ROOT", [](double a, double b) -> double { return std::pow(b, 1.0/(int)a); }},
	{"POW", [](double a, double b) -> double { return std::pow(a, b); }},
	{"SIN", [](double a, double) -> double { return std::sin(a); }},
	{"COS", [](double a, double) -> double { return std::cos(a); }},
	{"TAN", [](double a, double) -> double { return std::tan(a); }},
};

NodeMath::NodeMath(
	NODE_MATH math,
	const v_slot_info_t&& in_slots,
	const v_slot_info_t&& out_slots
)
{
	PPK_ASSERT(math != +NODE_MATH::EMPTY, "Passsed NODE_MATH should not be EMPTY");

	Node::m_kind = m_kind;
	Node::m_name = Nodes::get_id(m_kind._to_string());
	m_math = math;
	m_in_slots = in_slots;
	m_out_slots = out_slots;

	if (m_math == +NODE_MATH::ROOT || m_math == +NODE_MATH::POW)
		m_needs_two_val = true;

	reset();

	PLOGD << "Created NodeMath: " << m_name << ", math = " << m_math._to_string();
}

void NodeMath::reset(void)
{
	switch (m_in_slots[0].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: m_first = (int)0; break;
		case NODE_SLOT::FLOAT: m_first = (float)0; break;
		case NODE_SLOT::DOUBLE: m_first = (double)0; break;
		case NODE_SLOT::STRING: break;
	}

	switch (m_in_slots[m_in_slots.size() - 1].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: m_second = (int)0; break;
		case NODE_SLOT::FLOAT: m_second = (float)0; break;
		case NODE_SLOT::DOUBLE: m_second = (double)0; break;
		case NODE_SLOT::STRING: break;
	}
}

const char* NodeMath::get_string_a(void)
{
	switch (m_math)
	{
		case NODE_MATH::EMPTY: break;
		case NODE_MATH::ROOT: return "Index:"; break;
		case NODE_MATH::POW: return "Base:"; break;
		case NODE_MATH::SIN:
		case NODE_MATH::COS:
		case NODE_MATH::TAN: return "Number:"; break;
	}

	PPK_ASSERT(false, "this should not be reached");
	return "";
}

const char* NodeMath::get_string_b(void)
{
	switch (m_math)
	{
		case NODE_MATH::EMPTY: break;
		case NODE_MATH::ROOT: return "Radicand:"; break;
		case NODE_MATH::POW: return "Exponent:"; break;
		case NODE_MATH::SIN: break;
		case NODE_MATH::COS: break;
		case NODE_MATH::TAN: break;
	}

	PPK_ASSERT(false, "this should not be reached");
	return "";
}

const char* NodeMath::get_icon(void)
{
	switch (m_math)
	{
		case NODE_MATH::EMPTY: break;
		case NODE_MATH::ROOT: return ICON_FA_SQUARE_ROOT_ALT; break;
		case NODE_MATH::POW: return ICON_FA_ANGLE_UP; break;
		case NODE_MATH::SIN: break;
		case NODE_MATH::COS: break;
		case NODE_MATH::TAN: break;
	}

	PPK_ASSERT(false, "this should not be reached");
	return "";
}

void NodeMath::draw_first(void)
{
	switch (m_in_slots[0].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: ImGui::Text("%d", std::get<int>(m_first)); break;
		case NODE_SLOT::FLOAT: ImGui::Text("%f", std::get<float>(m_first)); break;
		case NODE_SLOT::DOUBLE: ImGui::Text("%lf", std::get<double>(m_first)); break;
		case NODE_SLOT::STRING: break;
	}
}

void NodeMath::draw_second(void)
{
	switch (m_in_slots[m_in_slots.size() - 1].kind)
	{
		case NODE_SLOT::EMPTY: break;
		case NODE_SLOT::BOOL: break;
		case NODE_SLOT::INTEGER: ImGui::Text("%d", std::get<int>(m_second)); break;
		case NODE_SLOT::FLOAT: ImGui::Text("%f", std::get<float>(m_second)); break;
		case NODE_SLOT::DOUBLE: ImGui::Text("%lf", std::get<double>(m_second)); break;
		case NODE_SLOT::STRING: break;
	}
}
}
