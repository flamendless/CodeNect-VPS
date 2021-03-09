#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include "ui/new_project.hpp"

namespace CodeNect
{
struct ProjectMeta
{
	std::string title;
	std::string filepath;
	std::string author;
	std::string creation_dt;
};

struct Project
{
	static bool has_open_proj;
	static ProjectMeta meta;
	static NewProject new_proj;

	static void register_commands(void);
	static int on_create_new(
		const char* filename,
		const char* title,
		const char* author
	);
	static void init_new(void);
	static int open(void);
	static int parse(void);
	static void draw(void);
	static void close(void);
};
}

#endif //PROJECT_H
