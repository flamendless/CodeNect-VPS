#ifndef _ALERT_HPP
#define _ALERT_HPP

#include <functional>
#include <string>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "ui/docs.hpp"

namespace CodeNect
{
enum class ALERT_TYPE { SUCCESS, ERR, WARNING };

struct Alert
{
	static ALERT_TYPE type;
	static std::string message;
	static bool is_open;
	static bool has_cb;
	static DOC_ID doc_id;
	static ImGuiWindowFlags flags;

	static std::function<void(void)> fn_custom_draw;

	Alert() = delete;
	static int init(void);
	static bool keypress(int key, int scancode, int mods);
	static void open(ALERT_TYPE type, std::string msg);
	static void open_for_docs(std::string msg, DOC_ID);
	static void close(void);
	static void draw(void);
};
}

#endif //_ALERT_HPP
