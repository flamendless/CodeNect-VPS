#ifndef NODE_INTERFACE_HPP
#define NODE_INTERFACE_HPP

#include "ImNodesEz.h"

namespace CodeNect
{
struct NodeInterface
{
	static bool is_open;
	static bool has_open_file;
	static ImVec2 pos;
	static ImVec2 size;

	NodeInterface() = delete;
	static bool init();
	static void draw();
	static void draw_startup();
	static void draw_main();
};
}

#endif //NODE_INTERFACE_HPP
