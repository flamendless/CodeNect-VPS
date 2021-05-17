#ifndef _COMMANDS_HPP
#define _COMMANDS_HPP

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
	bool m_close_command_palette = false;

	enum class TAG { NONE, _VOID, _INT } m_tag = TAG::NONE;
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
		m_tag = TAG::_VOID;
	}

	void set_fn(int (*fn)(void))
	{
		m_fn_int = fn;
		m_tag = TAG::_INT;
	}

	void run() const
	{
		switch(m_tag)
		{
			case Command::TAG::NONE: break;
			case Command::TAG::_VOID: m_fn_void(); break;
			case Command::TAG::_INT: m_fn_int(); break;
		}
	}
};

struct Commands
{
	Commands() = delete;
	static std::vector<const Command*> v_cmd;
	static void register_cmd(const Command& cmd);
	static void shutdown(void);
};
}

#endif //_COMMANDS_HPP
