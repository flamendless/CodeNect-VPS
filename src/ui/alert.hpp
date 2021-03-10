#ifndef ALERT_HPP
#define ALERT_HPP

#include <string>
#include "imgui.h"

namespace CodeNect
{
enum ALERT_TYPE
{
	SUCCESS,
	ERROR,
};

struct Alert
{
	static ALERT_TYPE type;
	static std::string message;
	static bool is_open;
	static ImGuiWindowFlags flags;

	Alert() = delete;
	static void open(ALERT_TYPE type, std::string msg);
	static void draw();
};
}

#endif //ALERT_HPP
