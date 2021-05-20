#ifndef _ASSESSMENTS_HPP
#define _ASSESSMENTS_HPP

#include <vector>
#include <string>

namespace CodeNect
{
struct Assessment
{
	const char* title;
	std::string str_content;
	std::vector<std::string> v_expected;
	std::vector<std::string> v_submission;
};


struct AssessmentResult
{
	Assessment assessment;
	int score = 0;
	std::vector<int> v_lines_diff;
};

struct Assessments
{
	static std::vector<Assessment> v_list;
	static bool has_assessment;
	static Assessment current_assessment;
	static std::vector<AssessmentResult> v_results;

	Assessments() = delete;
	static void submit(std::vector<std::string>&);
	static int get_score(std::vector<std::string>&, std::vector<std::string>&, std::vector<int>&);
};
}

#endif //_ASSESSMENTS_HPP
