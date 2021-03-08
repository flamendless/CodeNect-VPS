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

	static void init();
	static int on_create_new(
		const char* filename,
		const char* title,
		const char* author
	);
	static void init_new();
	static int open();
	static int parse();
	static void draw();
	static void close();
};
}

#endif //PROJECT_H
