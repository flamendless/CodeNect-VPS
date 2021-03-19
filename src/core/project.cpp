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
#include "modules/node_var.hpp"
#include "modules/node_op.hpp"

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

	Command* cmd_save_project = new Command("Save Project", "save project file", ICON_FA_SAVE);
	cmd_save_project->set_fn(Project::save_cmd);
	cmd_save_project->m_close_command_palette = true;

	Commands::register_cmd(*cmd_new_project);
	Commands::register_cmd(*cmd_open_project);
	Commands::register_cmd(*cmd_save_project);
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

void Project::save_cmd(void)
{
	int res = Project::save();

	if (res == RES_FAIL)
		Alert::open(ALERT_TYPE::ERROR, "Failed to save project");
	else
		Alert::open(ALERT_TYPE::SUCCESS, "Successfully saved project");
}

int Project::save(void)
{
	if (!Project::has_open_proj)
		return RES_FAIL;

	PLOGI << "Saving project file...";

	CSimpleIniA ini;
	ini.SetUnicode();
	SI_Error res = ini.LoadFile(Project::meta.filepath.c_str());

	if (res < 0)
	{
		PLOGE << "File not found anymore";
		return RES_FAIL;
	}

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	//delete first the unnecessary sections
	for (CSimpleIniA::Entry& section : sections)
	{
		std::string str_section = section.pItem;

		if (std::string(str_section).find("meta") == 0)
			continue;

		ini.Delete(section.pItem, nullptr);
	}

	//save nodes
	int s_i = 0;

	for (Node* node : Nodes::v_nodes)
	{
		std::string str_name = node->m_name;
		std::string str_section = std::string(PROJ_NODE_PREFIX) + str_name + "_" + std::to_string(s_i);
		const char* section = str_section.c_str();
		ini.SetValue(section, "name", str_name.c_str());

		const char* kind = node->m_kind._to_string();
		ini.SetValue(section, "kind", kind);

		std::string str_x = std::to_string(node->m_pos.x);
		std::string str_y = std::to_string(node->m_pos.y);
		ini.SetValue(section, "x", str_x.c_str());
		ini.SetValue(section, "y", str_y.c_str());

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = (NodeVariable*)node;
				const char* value_slot = node_var->m_value_orig.m_slot._to_string();
				std::string str_value = node_var->m_value_orig.get_value_str();
				ini.SetValue(section, "value_slot", value_slot);
				ini.SetValue(section, "value", str_value.c_str());
				break;
			}
			case NODE_KIND::OPERATION:
			{
				NodeOp* node_op = (NodeOp*)node;
				const char* op = node_op->m_op._to_string();
				ini.SetValue(section, "op", op);
				break;
			}
			case NODE_KIND::IF: break;
		}

		int in_i = 1;
		int out_i = 1;

		for (ImNodes::Ez::SlotInfo& slot : node->m_in_slots)
		{
			std::string str_in = std::string(PROJ_INPUT_SLOT_PREFIX) + std::to_string(in_i);
			const char* in_val = slot.title;
			ini.SetValue(section, str_in.c_str(), in_val);
			in_i++;
		}

		for (ImNodes::Ez::SlotInfo& slot : node->m_out_slots)
		{
			std::string str_out = std::string(PROJ_OUTPUT_SLOT_PREFIX) + std::to_string(out_i);
			const char* out_val = slot.title;
			ini.SetValue(section, str_out.c_str(), out_val);
			out_i++;
		}

		s_i++;
	}

	if (ini.SaveFile(Project::meta.filepath.c_str()) < 0)
	{
		PLOGE << "Failed to save project to disk";
		return RES_FAIL;
	}

	PLOGI << "Saved project file";

	return  RES_SUCCESS;
}

int Project::parse(void)
{
	PLOGI << "Parsing project file...";

	CSimpleIniA ini;
	ini.SetUnicode();
	SI_Error res = ini.LoadFile(Project::meta.filepath.c_str());

	if (res < 0)
	{
		PLOGE << "Can't parse project file";
		return RES_FAIL;
	}

	Project::meta.title = ini.GetValue("meta", "title", "");
	glfwSetWindowTitle(App::window, Project::meta.title.c_str());
	PLOGV << "Project Title: " << Project::meta.title;

	Project::meta.author = ini.GetValue("meta", "author", "");
	PLOGV << "Project Author: " << Project::meta.author;

	Project::meta.creation_dt = ini.GetValue("meta", "creation_dt", "");
	PLOGV << "Project Creation Datetime: " << Project::meta.creation_dt;

	int res_nodes = Project::parse_nodes(ini);

	if (res_nodes == RES_FAIL)
	{
		PLOGE << "Can't parse project nodes";
		return RES_FAIL;
	}

	PLOGI << "Parsed project file successfully";

	return RES_SUCCESS;
}

int Project::parse_nodes(CSimpleIniA& ini)
{
	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	std::vector<NodeMeta*> node_meta;

	for (CSimpleIniA::Entry& section : sections)
	{
		std::string str_section = section.pItem;

		if (std::string(str_section).find(PROJ_NODE_PREFIX, 0) == 0)
		{
			const char* name = ini.GetValue(section.pItem, "name");
			const char* kind = ini.GetValue(section.pItem, "kind");
			const char* value = ini.GetValue(section.pItem, "value", "0");
			const char* value_slot = ini.GetValue(section.pItem, "value_slot", "EMPTY");
			const char* op = ini.GetValue(section.pItem, "op", "EMPTY");
			const float x = std::stof(ini.GetValue(section.pItem, "x", "300"));
			const float y = std::stof(ini.GetValue(section.pItem, "y", "300"));

			NodeMeta* nm = new NodeMeta();
			nm->m_name = name;
			nm->m_kind = kind;
			nm->m_value = value;
			nm->m_value_slot = value_slot;
			nm->m_op = op;
			nm->x = x;
			nm->y = y;

			//get input/output slots
			CSimpleIniA::TNamesDepend keys;
			ini.GetAllKeys(section.pItem, keys);

			for (CSimpleIniA::Entry& key : keys)
			{
				//find input slots first
				if (std::string(key.pItem).find(PROJ_INPUT_SLOT_PREFIX, 0) == 0)
				{
					const char* slot = ini.GetValue(section.pItem, key.pItem);
					nm->m_input_slots.push_back(slot);
				}

				//find output slots second
				if (std::string(key.pItem).find(PROJ_OUTPUT_SLOT_PREFIX, 0) == 0)
				{
					const char* slot = ini.GetValue(section.pItem, key.pItem);
					nm->m_output_slots.push_back(slot);
				}
			}

			node_meta.push_back(nm);
		}
		else if (str_section.find(PROJ_CONNECTION_PREFIX, 0) == 0)
		{
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
