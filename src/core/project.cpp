#include "core/project.hpp"

#include <filesystem>
#include <ctime>
#include <GLFW/glfw3.h>
#include "plog/Log.h"
#include "modules/filesystem.hpp"
#include "core/app.hpp"
#include "core/defines.hpp"
#include "core/utils.hpp"
#include "core/commands.hpp"
#include "ui/alert.hpp"
#include "modules/node_meta.hpp"
#include "modules/nodes.hpp"

namespace CodeNect
{
#if DEBUG_MODE
bool Project::has_open_proj = true;
#else
bool Project::has_open_proj = false;
#endif
ProjectMeta Project::meta {};
NewProject Project::new_proj;

void Project::register_commands(void)
{
	Command* cmd_new_project = new Command("New Project", "create a new project", ICON_FA_FILE);
	cmd_new_project->set_fn(Project::init_new);
	cmd_new_project->m_close_command_palette = true;

	Command* cmd_open_project = new Command("Open Project", "open a project from file", ICON_FA_FOLDER_OPEN);
	cmd_open_project->set_fn(Project::open);
	cmd_open_project->m_close_command_palette = true;

	Commands::register_cmd(*cmd_new_project);
	Commands::register_cmd(*cmd_open_project);
}

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

void Project::init_new(void)
{
	//fill Project data from args
	Project::new_proj.m_on_create = &Project::on_create_new;
	Project::new_proj.m_is_open = true;
}

int Project::open(void)
{
	bool res = Filesystem::open_project_file(Project::meta.filepath);

	if (res)
	{
		PLOGV << "Opened: " << Project::meta.filepath;

		if (Project::parse() == RES_FAIL)
			return RES_FAIL;

		Project::has_open_proj = true;

		return RES_SUCCESS;
	}

	return RES_FAIL;
}

int Project::open(const char* filename)
{
	Project::meta.filepath = filename;

	if (Project::parse() == RES_FAIL)
		return RES_FAIL;

	Project::has_open_proj = true;

	return RES_FAIL;
}

int Project::parse(void)
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

	int res_nodes = Project::parse_nodes(reader);

	if (res_nodes == RES_FAIL)
	{
		PLOGE << "Can't parse project nodes";
		return RES_FAIL;
	}

	PLOGI << "Parsed project file successfully";

	return RES_SUCCESS;
}

int Project::parse_nodes(CSimpleIniA& reader)
{
	CSimpleIniA::TNamesDepend sections;
	reader.GetAllSections(sections);

	std::vector<NodeMeta*> node_meta;

	for (CSimpleIniA::Entry& section : sections)
	{
		if (std::string(section.pItem).find(PROJ_NODE_PREFIX, 0) == 0)
		{
			const char* name = reader.GetValue(section.pItem, "name");
			const char* kind = reader.GetValue(section.pItem, "kind");
			const char* value = reader.GetValue(section.pItem, "value");
			const char* value_slot = reader.GetValue(section.pItem, "value_slot");
			const float x = std::stof(reader.GetValue(section.pItem, "x"));
			const float y = std::stof(reader.GetValue(section.pItem, "y"));

			NodeMeta* nm = new NodeMeta();
			nm->m_name = name;
			nm->m_kind = kind;
			nm->m_value = value;
			nm->m_value_slot = value_slot;
			nm->x = x;
			nm->y = y;

			//get input/output slots
			CSimpleIniA::TNamesDepend keys;
			reader.GetAllKeys(section.pItem, keys);

			for (CSimpleIniA::Entry& key : keys)
			{
				//find input slots first
				if (std::string(key.pItem).find(PROJ_INPUT_SLOT_PREFIX, 0) == 0)
				{
					const char* slot = reader.GetValue(section.pItem, key.pItem);
					nm->m_input_slots.push_back(slot);
				}

				//find output slots second
				if (std::string(key.pItem).find(PROJ_OUTPUT_SLOT_PREFIX, 0) == 0)
				{
					const char* slot = reader.GetValue(section.pItem, key.pItem);
					nm->m_output_slots.push_back(slot);
				}
			}

			node_meta.push_back(nm);
		}
	}

	Nodes::build_from_meta(node_meta);

	for (NodeMeta* nm : node_meta)
	{
		nm->m_input_slots.clear();
		nm->m_output_slots.clear();
	}

	node_meta.clear();

	return RES_SUCCESS;
}

void Project::draw(void)
{
	if (Project::new_proj.m_is_open)
	{
		ImGui::OpenPopup("NewProjectPopup");
		Project::new_proj.draw();
	}
}

void Project::close(void)
{
	Project::has_open_proj = false;

	Project::meta.filepath.clear();
	Project::meta.title.clear();
	Project::meta.author.clear();
	Project::meta.creation_dt.clear();

	PLOGV << "Project closed";
}
}
