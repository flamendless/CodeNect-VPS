#ifndef _TRANSPILER_HPP
#define _TRANSPILER_HPP

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "libtcc.h"
#include "core/defines.hpp"
#include "node/node.hpp"

namespace CodeNect
{
enum class OUTPUT_TYPE { NORMAL = 1, SUCCESS, ERROR, PROMPT };

struct Transpiler
{
	static TCCState* tcc_state;
	static std::string output_code;
	static std::string runnable_code;
	static std::vector<std::pair<std::string, OUTPUT_TYPE>> v_output;
	static std::vector<std::string> v_declarations;
	static std::map<std::string, bool> m_temp_names;
	static int level;
	static bool has_ran;
	static bool has_compiled;

	Transpiler() = delete;
	static int init(void);
	static void register_commands(void);
	static void error(const char* str);
	static std::string get_temp_name(const char* name);
	static void set_pre_entry(std::string& str_incl, std::string& str_structs, bool is_tcc);
	static void build_runnable_code(std::string& out, bool is_tcc);
	static void transpile_decls(std::vector<Node*>& v, std::string& output);
	static void transpile(std::vector<Node*>& v, std::string& output);
	static int compile(void);
	static int run(void);
	static void clear(void);
	static void save_file(void);
	static void shutdown(void);
};
}

#endif //_TRANSPILER_HPP
