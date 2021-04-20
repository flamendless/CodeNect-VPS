#ifndef _TCC_SYMBOLS_HPP
#define _TCC_SYMBOLS_HPP

#include <iostream>
#include <cstring>
#include <stdarg.h>
#include "libtcc.h"
#include "plog/Log.h"
#include "fmt/format.h"
#include "fmt/printf.h"
#include "modules/transpiler.hpp"

namespace CodeNect::TCCSymbols
{
inline void send_string(const char* str)
{
	char* temp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(temp, str);
	temp[strlen(str) - 1] = '\0';

	//write length of string to pass
	int len = strlen(temp);
	if (write(Transpiler::pipefds[1], &len, sizeof(int)) < 0)
	{
		PLOGE << "Failed to write length of string from child to pipe";
	}

	//write string to pass
	if (write(Transpiler::pipefds[1], str, sizeof(char) * len) < 0)
	{
		PLOGE << "Failed to write string to pipe";
	}
}

inline void read_string(char* buffer)
{
	int len;
	if (read(Transpiler::pipefds2[0], &len, sizeof(int)) < 0)
	{
		PLOGE << "Failed to read length of string from parent";
	}

	if (read(Transpiler::pipefds2[0], buffer, len) < 0)
	{
		PLOGE << "Failed to read string from parent";
	}
}

inline void cn_print(const char* str)
{
	send_string(str);
	printf("%s", str);
}

inline void cn_printf(char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	std::string str = fmt::sprintf(format, fmt::make_printf_args(arg));
	send_string(str.c_str());
	printf("%s", str.c_str());
}

inline void cn_prompt(const char* str, char* out_buffer)
{
	send_string(str);
	printf("%s", str);
	read_string(out_buffer);
}

inline void register_symbols(TCCState* state)
{
	tcc_add_symbol(state, "cn_print", (const void*)cn_print);
	tcc_add_symbol(state, "cn_printf", (const void*)cn_printf);
	tcc_add_symbol(state, "cn_prompt", (const void*)cn_prompt);
}
}

#endif //_TCC_SYMBOLS_HPP
