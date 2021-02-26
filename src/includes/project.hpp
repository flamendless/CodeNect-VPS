#ifndef PROJECT_H
#define PROJECT_H

#include <string>

namespace CodeNect
{
struct Project
{
	static std::string title;
	static std::string filepath;
	static std::string author;

	static void init_new(
			const std::string& title,
			const std::string& author
			//add datetime
		);
	static int open();
	static int parse();
};
}

#endif //PROJECT_H
