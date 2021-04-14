#ifndef _TRANSPILER_HPP
#define _TRANSPILER_HPP

#include <functional>
#include <string>
#include <vector>
#include "libtcc.h"
#include "core/defines.hpp"

namespace CodeNect
{
enum class OUTPUT_TYPE { NORMAL = 1, SUCCESS, ERROR };

struct Transpiler
{
	static TCCState* tcc_state;
	static std::string code;
	static std::string output_code;
	static std::vector<std::pair<std::string, OUTPUT_TYPE>> v_output;
	static std::vector<std::string> v_declarations;
	static std::function<void(char**, int*)> fn;

	Transpiler() = delete;
	static int init(void);
	static void register_commands(void);
	static void build_code(void);
	static int compile(void);
	static int run(void);
	static void clear(void);
	static void save_file(void);
	static void shutdown(void);
};
}

#endif //_TRANSPILER_HPP
