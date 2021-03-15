#ifndef VALIDATIONS_HPP
#define VALIDATIONS_HPP

#include "verbalexpressions.hpp"
#include "core/defines.hpp"

namespace CodeNect
{
namespace Validations
{
inline std::vector<std::string> v_reserved_keywords
{
	"bool", "int", "float", "double", "string", "char", "void"
};

inline verex::verex expr_var_name = verex::verex().search_one_line().start_of_line()
	.then("(_*)?[a-zA-Z]+(_*)?((_*[0-9a-zA-Z]+)*)?").end_of_line();

inline int validate_var_name(const char* str)
{
	std::string str_arg(str);

	for (const std::string& keyword : v_reserved_keywords)
	{
		if (keyword == str_arg)
			return RES_VARNAME_CONFLICT_KEYWORD;
	}

	bool res = expr_var_name.test(str);

	if (!res)
		return RES_VARNAME_INVALID;

	return RES_VARNAME_VALID;
}
}
}

#endif //VALIDATIONS_HPP
