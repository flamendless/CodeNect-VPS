#ifndef _NODE_COLORS_HPP
#define _NODE_COLORS_HPP

#include <map>
#include <string>
#include "imgui.h"

namespace CodeNect
{
struct NodeColors
{
	struct Lookup
	{
		static ImVec4 GREEN;
		static ImVec4 YELLOW;
		static ImVec4 ORANGE;
		static ImVec4 RED;
		static ImVec4 PINK;
		static ImVec4 VIOLET;
		static ImVec4 BLUE;
		static ImVec4 LIGHT_BLUE;
	};

	struct Connection
	{
		static ImVec4 DEFAULT;
		static ImVec4 HOVERED;
		static ImVec4 TRUE;
		static ImVec4 FALSE;
	};

	static std::map<std::string, ImVec4> m_kind;
};
}

#endif //_NODE_COLORS_HPP