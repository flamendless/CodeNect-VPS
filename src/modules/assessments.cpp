#include "modules/assessments.hpp"

namespace CodeNect
{
Assessment Assessments::current_assessment;
bool Assessments::has_assessment = false;
std::vector<AssessmentResult> Assessments::v_results;

std::string md_ask_user_name =
#include "markdown/a_ask_user_name.md"
;
std::string md_basic_op =
#include "markdown/a_basic_op.md"
;
std::string md_string_concat =
#include "markdown/a_string_concat.md"
;
std::string md_hypotenuse =
#include "markdown/a_hypotenuse.md"
;
std::string md_summation =
#include "markdown/a_summation.md"
;

std::vector<Assessment> Assessments::v_list = {
	{"Ask User for Name", std::move(md_ask_user_name),
		{
			"What is your name? Brandon",
			"Hello there, Brandon!",
		}
	},
	{"Basic Operations", std::move(md_basic_op),
		{
			"Please enter the first number: 26",
			"Please enter the second number: 2",
			"Sum of the two numbers is: 28",
			"Difference of the two numbers is: 24",
			"Product of the two numbers is: 52",
			"Quotient of the two numbers is: 13",
		}
	},
	{"String Concatenation", std::move(md_string_concat),
		{
			"What is your first name? Brandon",
			"What is your middle name? Blanker",
			"What is your last name? Lim-it",
			"Hello there, Brandon Blanker Lim-it!",
		}
	},
	{"Getting the Hypotenuse", std::move(md_hypotenuse),
		{
			"Enter the value of a: 3",
			"Enter the value of b: 4",
			"Hypotenuse is: 5",
		}
	},
	{"Getting the Summation of Two Numbers", std::move(md_summation),
		{
			"Enter starting number: 5",
			"Enter ending number: 10",
			"Summation: 45",
		}
	},
};

void Assessments::submit(std::vector<std::string>& v_lines)
{
	AssessmentResult res;
	Assessments::current_assessment.v_submission = v_lines;
	int score = Assessments::get_score(Assessments::current_assessment.v_submission,
			Assessments::current_assessment.v_expected, res.v_lines_diff);
	res.score = score;
	Assessments::v_results.push_back(res);
}

int Assessments::get_score(std::vector<std::string>& v_submitted,
		std::vector<std::string>& v_expected,
		std::vector<int>& v_lines_diff)
{
	int score = 0;

	for (unsigned long i = 0; i < v_submitted.size(); i++)
	{
		std::string& str_submitted = v_submitted[i];
		std::string& str_expected = v_expected[i];
		if (str_submitted.compare(str_expected) == 0)
			score++;
		else
			v_lines_diff.push_back(i);
	}
	return score;
}
}
