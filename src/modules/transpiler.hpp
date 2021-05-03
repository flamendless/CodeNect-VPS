#ifndef _TRANSPILER_HPP
#define _TRANSPILER_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "libtcc.h"
#include "core/defines.hpp"
#include "node/node.hpp"
#include "core/message_info.hpp"

namespace CodeNect
{
struct State
{
	Node* node_branch = nullptr;
	std::vector<std::vector<Node*>> v_seq;
	std::vector<Node*> v_rest;
};

struct Transpiler
{
	static TCCState* tcc_state;
	static std::string output_code;
	static std::string runnable_code;
	static std::vector<MessageInfo> v_output;
	static std::vector<std::string> v_declarations;
	static std::map<std::string, bool> m_temp_names;
	static std::map<std::string, bool> m_declared;
	static std::map<std::string, std::string> m_array_init; //array_name, free_method_name
	static int level;
	static bool has_ran;
	static bool has_compiled;
	static std::string recent_temp;

	Transpiler() = delete;
	static int init(void);
	static void register_commands(void);
	static void handle_error(void* opaque, const char* msg);
	static void add_message(const std::string&, OUTPUT_TYPE = OUTPUT_TYPE::SUCCESS, Node* = nullptr);
	static std::string get_temp_name(const char* name);
	static std::pair<std::string, bool> get_temp_name(const char* name, bool reuse);
	static bool is_valid_decls(Node*);
	static void set_pre_entry(std::string& str_incl, std::string& str_structs, bool is_tcc);
	static void build_runnable_code(std::string& out, bool is_tcc);
	static void transpile_decls(std::vector<Node*>& v, std::string& output);
	static void transpile_decls_array(std::vector<Node*>& v, std::string& output);
	static Node* transpile(std::vector<Node*>& v, std::string& output);
	static void arrange_v(std::vector<Node*>&);
	static std::vector<Node*> get_sequence(Node* start_node);
	static std::vector<std::vector<Node*>> get_v_sequence(State*);
	static std::vector<Node*> get_rest(State*);
	static int compile(void);
	static int run(void);
	static void clear(void);
	static void save_file(void);
	static void shutdown(void);
};
}

#endif //_TRANSPILER_HPP
