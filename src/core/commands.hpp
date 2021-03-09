#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <functional>

namespace CodeNect
{
struct Command
{
	std::string title;
	std::string desc;

	enum { NONE, VOID, INT } tag = NONE;
	union
	{
		void (*fn_void)(void);
		int (*fn_int)(void);
	};

	Command(const char* title, const char* desc)
		: title {title}, desc {desc}
	{}

	void set_fn(void (*fn)(void))
	{
		fn_void = fn;
		tag = VOID;
	}

	void set_fn(int (*fn)())
	{
		fn_int = fn;
		tag = INT;
	}
};

struct Commands
{
	static std::vector<Command> v_cmd;
	static void register_cmd(const Command& cmd);
};
}

#endif //COMMANDS_HPP
