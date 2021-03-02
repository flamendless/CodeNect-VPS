#include "project.hpp"

#include <ctime>
#include <GLFW/glfw3.h>
#include "SimpleIni.h"
#include "plog/Log.h"
#include "app.hpp"
#include "filesystem.hpp"
#include "defines.hpp"

namespace CodeNect
{
bool Project::has_open_proj = false;
ProjectMeta Project::meta {};
PopupNewProject Project::popup_new_proj;

PopupAlert popup_alert;

void Project::on_create_new(const char* filename, const char* title, const char* author)
{
	CSimpleIniA writer;
	writer.SetUnicode();

	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);

	writer.SetValue("meta", "title", title);
	writer.SetValue("meta", "author", author);
	writer.SetValue("meta", "creation_dt", buffer);

	std::string file = std::string(filename) + "." + PROJECT_EXT;
	int res = writer.SaveFile(file.c_str());

	if (res < 0)
	{
		popup_alert.open(ALERT_TYPE::ERROR, "Failed to create new project");
		PLOGW << "Creation of new project failed";
	}
	else
	{
		popup_alert.open(ALERT_TYPE::SUCCESS, "New Project created successfully.\nPlease open the created project file");
		PLOGV << "Creation of new project successful";
	}
}

void Project::init_new()
{
	//fill Project data from args
	Project::popup_new_proj.set_center_pos();
	Project::popup_new_proj.m_on_create = &Project::on_create_new;
	Project::popup_new_proj.m_is_open = true;
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
	if (Project::popup_new_proj.m_is_open)
	{
		ImGui::OpenPopup("NewProjectPopup");
		Project::popup_new_proj.draw();
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
