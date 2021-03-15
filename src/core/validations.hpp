#ifndef VALIDATIONS_HPP
#define VALIDATIONS_HPP

#include <map>
#include "plog/Log.h"
#include "verbalexpressions.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
namespace Validations
{
inline std::map<const std::string, bool> m_reserved_keywords
{
	{"bool", true},
	{"int", true},
	{"float", true},
	{"double", true},
	{"string", true},
	{"char", true},
};

inline verex::verex expr_var_name = verex::verex().search_one_line().start_of_line()
	.then("(_*)?[a-zA-Z]+(_*)?((_*[0-9a-zA-Z]+)*)?").end_of_line();

inline int validate_var_name(const char* str)
{
	if (m_reserved_keywords.count(str))
		return RES_VARNAME_CONFLICT_KEYWORD;

	bool res = expr_var_name.test(str);

	if (!res)
		return RES_VARNAME_INVALID;

	return RES_VARNAME_VALID;
}
}
}

#endif //VALIDATIONS_HPP
