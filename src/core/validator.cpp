#include "core/validator.hpp"

#include <map>
#include "plog/Log.h"
#include "verbalexpressions.hpp"
#include "core/defines.hpp"
#include "node/nodes.hpp"

namespace CodeNect::Validator
{
std::map<const std::string, bool> m_reserved_keywords
{
	{"bool", true},
	{"int", true},
	{"float", true},
	{"double", true},
	{"string", true},
	{"char", true},
};

verex::verex expr_var_name = verex::verex().search_one_line().start_of_line()
	.then("(_*)?[a-zA-Z]+(_*)?((_*[0-9a-zA-Z]+)*)?").end_of_line();

bool check_var_name_taken(const char* str)
{
	for (Node* node : Nodes::v_nodes)
	{
		if (std::strcmp(node->m_name, str) == 0)
			return true;
	}

	return false;
}

int validate_var_name(const char* str)
{
	if (m_reserved_keywords.count(str))
		return RES_VARNAME_CONFLICT_KEYWORD;

	if (!expr_var_name.test(str))
		return RES_VARNAME_INVALID;

	if (check_var_name_taken(str))
		return RES_VARNAME_ALREADY_TAKEN;

	return RES_VARNAME_VALID;
}
}
