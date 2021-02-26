#include "project.hpp"

#include <GLFW/glfw3.h>
#include "SimpleIni.h"
#include "plog/Log.h"
#include "app.hpp"
#include "filesystem.hpp"
#include "defines.hpp"

namespace CodeNect
{
std::string Project::title;
std::string Project::filepath;
std::string Project::author;

void Project::init_new(const std::string& title, const std::string& author)
{
	//fill Project data from args
}

int Project::open()
{
	bool res = Filesystem::open_project_file(Project::filepath);

	if (res)
	{
		PLOGV << "Opened: " << Project::filepath;

		if (Project::parse() == RES_FAIL)
			return RES_FAIL;

		return RES_SUCCESS;
	}

	return RES_FAIL;
}

int Project::parse()
{
	PLOGI << "Parsing project file...";

	CSimpleIniA reader;
	reader.SetUnicode();
	SI_Error res = reader.LoadFile(Project::filepath.c_str());

	if (res < 0)
	{
		PLOGE << "Can't parse project file";

		return RES_FAIL;
	}

	Project::title = reader.GetValue("meta", "title", "");
	glfwSetWindowTitle(App::window, Project::title.c_str());
	PLOGV << "Project Title: " << Project::title;

	Project::author = reader.GetValue("meta", "author", "");
	PLOGV << "Project Author: " << Project::author;

	PLOGI << "Parsed project file successfully";

	return RES_SUCCESS;
}
}
