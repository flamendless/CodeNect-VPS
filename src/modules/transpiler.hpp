#ifndef _TRANSPILER_HPP
#define _TRANSPILER_HPP

#include <functional>
#include <string>
#include <vector>
#include "libtcc.h"
#include "core/defines.hpp"

namespace CodeNect
{
enum class OUTPUT_TYPE { NORMAL = 1, SUCCESS, ERROR, PROMPT };

struct Transpiler
{
	static TCCState* tcc_state;
	static std::string code;
	static std::string output_code;
	static std::vector<std::pair<std::string, OUTPUT_TYPE>> v_output;
	static std::vector<std::string> v_declarations;
	static std::function<void()> fn;
	static int level;
	static int pipefds[2]; //child to parent
	static int pipefds2[2]; //parent to child
	static pid_t pid_child;

	Transpiler() = delete;
	static int init(void);
	static void register_commands(void);
	static void error(const char* str);
	static void set_pre_entry(std::string& str_incl, std::string& str_structs);
	static void build_runnable_code(void);
	static void build_out_code(void);
	static int compile(void);
	static int run(void);
	static void clear(void);
	static void save_file(void);
	static void shutdown(void);
};
}

#endif //_TRANSPILER_HPP
