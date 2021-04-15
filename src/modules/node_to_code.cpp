#include "modules/node_to_code.hpp"

namespace CodeNect::NodeToCode
{
std::string node_var(NodeVariable* node_var)
{
	NodeValue* value = &node_var->m_value_orig;
	std::string type = value->get_type_str();
	const char* name = node_var->m_name;
	std::string val = value->get_value_str_ex();
	return fmt::format("{} {} = {}", type, name, val).append("\n");
}
}
