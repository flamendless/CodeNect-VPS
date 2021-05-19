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
	std::vector<std::string> v_output;
};

struct Assessments
{
	static std::vector<Assessment> v_list;
	static bool has_assessment;
	static Assessment current_assessment;

	Assessments() = delete;
	static bool compare_outputs(std::vector<std::string>&);
};
}

#endif //_ASSESSMENTS_HPP
