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
#include "node/node_meta.hpp"
#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cmp.hpp"

namespace CodeNect
{
#if DEBUG_MODE
bool Project::has_open_proj = true;
#else
bool Project::has_open_proj = false;
#endif
unsigned int Project::nodes_count = 0;
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

	Nodes::v_nodes.clear();
	Project::open(file.c_str());

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
		Nodes::reset();

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
	Nodes::reset();

	if (Project::parse() == RES_FAIL)
		return RES_FAIL;

	Project::has_open_proj = true;

	return RES_FAIL;
}

void Project::save_cmd(void)
{
	int res = Project::save();
	Project::nodes_count = Nodes::v_nodes.size();

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
		std::string str_section = std::string(PROJ_NODE_PREFIX) + std::to_string(s_i);
		const char* section = str_section.c_str();
		ini.SetValue(section, "name", node->m_name);

		const char* kind = node->m_kind._to_string();
		ini.SetValue(section, "kind", kind);

		std::string str_x = std::to_string(node->m_pos.x);
		std::string str_y = std::to_string(node->m_pos.y);
		ini.SetValue(section, "x", str_x.c_str());
		ini.SetValue(section, "y", str_y.c_str());
		ini.SetValue(section, "desc", node->m_desc);

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = static_cast<NodeVariable*>(node);
				const char* value_slot = node_var->m_value_orig.m_slot._to_string();
				std::string str_value = node_var->m_value_orig.get_value_str();
				ini.SetValue(section, "value_slot", value_slot);
				ini.SetValue(section, "value", str_value.c_str());
				break;
			}
			case NODE_KIND::OPERATION:
			{
				NodeOperation* node_op = static_cast<NodeOperation*>(node);
				const char* op = node_op->m_op._to_string();
				ini.SetValue(section, "op", op);
				break;
			}
			case NODE_KIND::CAST: break;
			case NODE_KIND::COMPARISON:
			{
				NodeComparison* node_cmp = static_cast<NodeComparison*>(node);
				const char* cmp = node_cmp->m_cmp._to_string();
				ini.SetValue(section, "cmp", cmp);
				break;
			}
			case NODE_KIND::IF: break;
		}

		//save input and output slots
		Project::save_slots(ini, section, node->m_in_slots, PROJ_INPUT_SLOT_PREFIX);
		Project::save_slots(ini, section, node->m_out_slots, PROJ_OUTPUT_SLOT_PREFIX);

		//save connections
		Project::save_connections(ini, node->m_connections);

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

void Project::save_slots(CSimpleIniA& ini, const char* section, v_slot_info_t& slots, const char* prefix)
{
	int i = 1;

	for (ImNodes::Ez::SlotInfo& slot : slots)
	{
		std::string str = std::string(prefix) + std::to_string(i);
		const char* val = slot.title;
		ini.SetValue(section, str.c_str(), val);
		i++;
	}
}

void Project::save_connections(CSimpleIniA& ini, std::vector<Connection>& v_connections)
{
	for (Connection& connection : v_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		Node* out_node = static_cast<Node*>(connection.out_node);

		std::string str_id = std::string(in_node->m_name) + "_" + std::string(out_node->m_name);
		std::string str_section = std::string(PROJ_CONNECTION_PREFIX) + str_id;
		const char* section = str_section.c_str();

		ini.SetValue(section, "in_node_name", in_node->m_name);
		ini.SetValue(section, "in_slot", connection.in_slot);
		ini.SetValue(section, "out_node_name", out_node->m_name);
		ini.SetValue(section, "out_slot", connection.out_slot);
	}
}

int Project::parse(void)
{
	PLOGI << "Parsing project file..." << Project::meta.filepath;

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

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);
	std::vector<NodeMeta*> v_node_meta;
	std::vector<ConnectionMeta*> v_connection_meta;

	for (CSimpleIniA::Entry& section : sections)
	{
		std::string str_section = section.pItem;

		if (str_section.find(PROJ_NODE_PREFIX, 0) == 0)
			Project::parse_nodes(ini, v_node_meta, section.pItem);
		else if (str_section.find(PROJ_CONNECTION_PREFIX, 0) == 0)
			Project::parse_connections(ini, v_connection_meta, section.pItem);
	}

	Nodes::build_from_meta(v_node_meta);
	Nodes::build_from_meta(v_connection_meta);

	//cleanup
	for (NodeMeta* nm : v_node_meta)
	{
		nm->m_input_slots.clear();
		nm->m_output_slots.clear();
	}

	v_node_meta.clear();
	v_connection_meta.clear();
	Project::nodes_count = Nodes::v_nodes.size();

	PLOGI << "Parsed project file successfully";

	return RES_SUCCESS;
}

void Project::parse_nodes(CSimpleIniA& ini, std::vector<NodeMeta*>& v_node_meta, const char* section)
{
	PLOGV << "Parsing node section: " << section;

	const char* name = ini.GetValue(section, "name");
	const char* kind = ini.GetValue(section, "kind");
	const float x = std::stof(ini.GetValue(section, "x", "300"));
	const float y = std::stof(ini.GetValue(section, "y", "300"));
	const char* desc = ini.GetValue(section, "desc", "");
	const char* value = ini.GetValue(section, "value", "0");
	const char* value_slot = ini.GetValue(section, "value_slot", "EMPTY");
	const char* op = ini.GetValue(section, "op", "EMPTY");
	const char* cmp = ini.GetValue(section, "cmp", "EMPTY");

	NodeMeta* nm = new NodeMeta();
	nm->m_name = name;
	nm->m_kind = kind;
	nm->x = x;
	nm->y = y;
	nm->m_desc = desc;
	nm->m_value = value;
	nm->m_value_slot = value_slot;
	nm->m_op = op;
	nm->m_cmp = cmp;

	//get input/output slots
	CSimpleIniA::TNamesDepend keys;
	ini.GetAllKeys(section, keys);

	for (CSimpleIniA::Entry& key : keys)
	{
		std::string str_key = key.pItem;

		//find input slots first
		if (str_key.find(PROJ_INPUT_SLOT_PREFIX, 0) == 0)
		{
			const char* slot = ini.GetValue(section, key.pItem);
			nm->m_input_slots.push_back(slot);
		}

		//find output slots second
		if (str_key.find(PROJ_OUTPUT_SLOT_PREFIX, 0) == 0)
		{
			const char* slot = ini.GetValue(section, key.pItem);
			nm->m_output_slots.push_back(slot);
		}
	}

	v_node_meta.push_back(nm);
}

void Project::parse_connections(CSimpleIniA& ini, std::vector<ConnectionMeta*>& v_connection_meta, const char* section)
{
	PLOGV << "Parsing connection section: " << section;

	const char* in_name = ini.GetValue(section, "in_node_name");
	const char* out_name = ini.GetValue(section, "out_node_name");
	const char* in_slot = ini.GetValue(section, "in_slot");
	const char* out_slot = ini.GetValue(section, "out_slot");

	ConnectionMeta* cm = new ConnectionMeta();
	cm->m_in_name = in_name;
	cm->m_in_slot = in_slot;
	cm->m_out_name = out_name;
	cm->m_out_slot = out_slot;

	v_connection_meta.push_back(cm);
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

bool Project::has_unsaved_changes(void)
{
	if (Project::nodes_count != Nodes::v_nodes.size())
	{
		Alert::open(ALERT_TYPE::ERROR, "There are unsaved changes. Are you sure you want to quti?");
		Alert::has_cb = true;
		Alert::fn_custom_draw = []()
		{
			if (ImGui::Button(ICON_FA_SIGN_OUT_ALT " Quit"))
			{
				Project::nodes_count = Nodes::v_nodes.size();
				glfwSetWindowShouldClose(App::window, GL_TRUE);
			}

			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_TIMES " Cancel"))
				Alert::close();
		};

		return true;
	}

	return false;
}
}
