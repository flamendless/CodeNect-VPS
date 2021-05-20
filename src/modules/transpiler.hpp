#ifndef _TRANSPILER_HPP
#define _TRANSPILER_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "libtcc.h"
#include "core/defines.hpp"
#include "node/node.hpp"
#include "node/node_branch.hpp"
#include "node/node_for.hpp"
#include "core/message_info.hpp"

namespace CodeNect
{
struct State
{
	NodeBranch* node_branch = nullptr;
	NodeFor* node_for = nullptr;
	bool is_in_else = false;
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
	static int n_transpiled;
	static std::vector<State> v_states;
	static std::vector<NodeBranch*> v_finished_branches;
	// static std::vector<std::string> v_lines;
	static std::vector<std::string> v_printed;

	Transpiler() = delete;
	static int init(void);
	static void register_commands(void);
	static void handle_error(void* opaque, const char* msg);
	static void add_message(const std::string&, OUTPUT_TYPE = OUTPUT_TYPE::SUCCESS, Node* = nullptr, DOC_ID = DOC_ID::EMPTY);
	static std::string get_temp_name(const char* name);
	static std::pair<std::string, bool> get_temp_name(const char* name, bool reuse);
	static bool is_valid_decls(Node*);
	static void set_pre_entry(std::string& str_incl, std::string& str_structs, bool is_tcc);
	static void build_runnable_code(std::string& out, bool is_tcc);
	static void transpile_decls(std::vector<Node*>& v, std::string& output);
	static void transpile_decls_array(std::vector<Node*>& v, std::string& output);
	static Node* transpile(std::vector<Node*>& v, std::string& output, State* = nullptr);
	static void arrange_v(std::vector<Node*>&);
	static std::vector<Node*> get_sequence(Node*, State*);
	static std::vector<std::vector<Node*>> get_v_sequence(State*);
	static std::vector<Node*> get_rest(State*);
	static int compile(void);
	static int run(void);
	// static int run_cmd(std::string&);
	static void clear(void);
	static void save_file(void);
	static void shutdown(void);
};
}

#endif //_TRANSPILER_HPP
