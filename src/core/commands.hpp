#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <functional>

namespace CodeNect
{
struct Command
{
	std::string m_title;
	std::string m_desc;
	const char* m_icon;

	enum { NONE, VOID, INT } m_tag = NONE;
	union
	{
		void (*m_fn_void)(void);
		int (*m_fn_int)(void);
	};

	Command(const char* title, const char* desc, const char* icon)
		: m_title {title}, m_desc {desc}, m_icon {icon}
	{}

	void set_fn(void (*fn)(void))
	{
		m_fn_void = fn;
		m_tag = VOID;
	}

	void set_fn(int (*fn)(void))
	{
		m_fn_int = fn;
		m_tag = INT;
	}

	void run() const
	{
		switch(m_tag)
		{
			case Command::NONE: break;
			case Command::VOID: m_fn_void(); break;
			case Command::INT: m_fn_int(); break;
		}
	}
};

struct Commands
{
	static std::vector<const Command*> v_cmd;
	static void register_cmd(const Command& cmd);
	static void shutdown(void);
};
}

#endif //COMMANDS_HPP
