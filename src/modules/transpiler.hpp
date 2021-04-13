#ifndef _TRANSPILER_HPP
#define _TRANSPILER_HPP

#include <functional>
#include <string>
#include <vector>
#include "libtcc.h"

namespace CodeNect
{
struct Transpiler
{
	static TCCState* tcc_state;
	static std::string code;
	static std::vector<std::string> v_output;
	static std::function<const char*(void)> fn;

	Transpiler() = delete;
	static int init(void);
	static void register_commands(void);
	static int compile(void);
	static int run(void);
	static void clear(void);
	static void shutdown(void);
};
}

#endif //_TRANSPILER_HPP
