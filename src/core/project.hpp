#ifndef _PROJECT_H
#define _PROJECT_H

#include <string>
#include "SimpleIni.h"
#include "node/connection.hpp"
#include "node/node_def.hpp"
#include "node/node_meta.hpp"
#include "node/node_array.hpp"
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
	static unsigned int nodes_count;
	static unsigned int connections_count;

	static bool has_open_proj;
	static ProjectMeta meta;
	static NewProject new_proj;

	Project() = delete;
	static void register_commands(void);
	static int on_create_new(
		const char* filename,
		const char* title,
		const char* author
	);
	static void init_new(void);
	static int open(void);
	static int open(const char* filename);
	static int save(void);
	static void save_slots(CSimpleIniA& ini, const char* section, v_slot_info_t& slots, const char* prefix);
	static void save_connections(CSimpleIniA& ini, std::vector<Connection>& v_connections);
	static void save_array_elements(CSimpleIniA& ini, const char* section, NodeArray* node_array);
	static void save_cmd(void);
	static int parse(void);
	static void parse_nodes(CSimpleIniA& ini, std::vector<NodeMeta*>& v_node_meta, const char* section);
	static void parse_connections(CSimpleIniA& ini, std::vector<ConnectionMeta*>& v_connection_meta, const char* section);
	static void draw(void);
	static void close(void);
	static bool has_unsaved_changes(void);
};
}

#endif //_PROJECT_H
