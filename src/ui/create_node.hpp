#ifndef CREATE_NODE_HPP
#define CREATE_NODE_HPP

#include <vector>
#include <string>
#include "imgui.h"
#include "IconsFontAwesome5.h"
#include "modules/node.hpp"

namespace CodeNect
{
enum NODE_KIND { VARIABLE = 1 };

struct Variable
{
	const char* m_name;
	const char* m_comment = "";
	NODE_SLOT m_type = NODE_SLOT::EMPTY;
	std::string m_value;
};

struct CreateNode
{
	static ImGuiWindowFlags flags;
	static bool is_open;
	static const char* title;
	static NODE_KIND kind;

	CreateNode() = delete;
	static void open(NODE_KIND kind);
	static void close(void);
	static void draw(void);
};
}

#endif //CREATE_NODE_HPP
