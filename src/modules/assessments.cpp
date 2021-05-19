#include "modules/assessments.hpp"

namespace CodeNect
{
Assessment Assessments::current_assessment;
bool Assessments::has_assessment = false;

std::string md_ask_user_name =
#include "markdown/a_ask_user_name.md"
;

std::vector<Assessment> Assessments::v_list = {
	{"Ask User for Name", std::move(md_ask_user_name),
		{
			"What is your name?",
			"Enter name: Brandon",
			"Hello there, Brandon!",
		}},
};

bool Assessments::compare_outputs(std::vector<std::string>&)
{
	return true;
}
}
