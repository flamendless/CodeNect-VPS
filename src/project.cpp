#include "modules/project.hpp"

#include <filesystem>
#include <ctime>
#include <GLFW/glfw3.h>
#include "SimpleIni.h"
#include "plog/Log.h"
#include "app.hpp"
#include "modules/filesystem.hpp"
#include "defines.hpp"
#include "utils.hpp"
#include "ui/alert.hpp"

namespace CodeNect
{
bool Project::has_open_proj = false;
ProjectMeta Project::meta {};
NewProject Project::new_proj;

int Project::on_create_new(const char* filename, const char* title, const char* author)
{
	CSimpleIniA writer;
	writer.SetUnicode();

	writer.SetValue("meta", "title", title);
	writer.SetValue("meta", "author", author);
	writer.SetValue("meta", "creation_dt", Utils::time_now().c_str());

	std::string file = filename;

	if (std::filesystem::path(filename).extension() != PROJECT_EXT_DOT)
		file += PROJECT_EXT_DOT;

	if (writer.SaveFile(file.c_str()) < 0)
		return RES_FAIL;

	return RES_SUCCESS;
}

void Project::init_new()
{
	//fill Project data from args
	Project::new_proj.m_on_create = &Project::on_create_new;
	Project::new_proj.m_is_open = true;
}

int Project::open()
{
	bool res = Filesystem::open_project_file(Project::meta.filepath);

	if (res)
	{
		PLOGV << "Opened: " << Project::meta.filepath;

		if (Project::parse() == RES_FAIL)
			return RES_FAIL;

		has_open_proj = true;

		return RES_SUCCESS;
	}

	return RES_FAIL;
}

int Project::parse()
{
	PLOGI << "Parsing project file...";

	CSimpleIniA reader;
	reader.SetUnicode();
	SI_Error res = reader.LoadFile(Project::meta.filepath.c_str());

	if (res < 0)
	{
		PLOGE << "Can't parse project file";

		return RES_FAIL;
	}

	Project::meta.title = reader.GetValue("meta", "title", "");
	glfwSetWindowTitle(App::window, Project::meta.title.c_str());
	PLOGV << "Project Title: " << Project::meta.title;

	Project::meta.author = reader.GetValue("meta", "author", "");
	PLOGV << "Project Author: " << Project::meta.author;

	Project::meta.creation_dt = reader.GetValue("meta", "creation_dt", "");
	PLOGV << "Project Creation Datetime: " << Project::meta.creation_dt;

	PLOGI << "Parsed project file successfully";

	return RES_SUCCESS;
}

void Project::draw()
{
	if (Project::new_proj.m_is_open)
	{
		ImGui::OpenPopup("NewProjectPopup");
		Project::new_proj.draw();
	}
}

void Project::close()
{
	has_open_proj = false;

	Project::meta.filepath.clear();
	Project::meta.title.clear();
	Project::meta.author.clear();
	Project::meta.creation_dt.clear();

	PLOGV << "Project closed";
}
}
