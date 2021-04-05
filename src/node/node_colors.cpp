#include "node/node_colors.hpp"

namespace CodeNect
{
#define ALPHA 1.0f

ImVec4 NodeColors::Lookup::GREEN = ImVec4((float)48/255, (float)163/255, (float)86/255, ALPHA);
ImVec4 NodeColors::Lookup::YELLOW = ImVec4((float)251/255, (float)221/255, (float)13/255, ALPHA);
ImVec4 NodeColors::Lookup::ORANGE = ImVec4((float)254/255, (float)171/255, (float)17/255, ALPHA);
ImVec4 NodeColors::Lookup::RED = ImVec4((float)251/255, (float)43/255, (float)12/255, ALPHA);
ImVec4 NodeColors::Lookup::PINK = ImVec4((float)252/255, (float)112/255, (float)170/255, ALPHA);
ImVec4 NodeColors::Lookup::VIOLET = ImVec4((float)216/255, (float)148/255, (float)248/255, ALPHA);
ImVec4 NodeColors::Lookup::BLUE = ImVec4((float)27/255, (float)2/255, (float)226/255, ALPHA);
ImVec4 NodeColors::Lookup::LIGHT_BLUE = ImVec4((float)19/255, (float)170/255, (float)251/255, ALPHA);

ImVec4 NodeColors::Connection::DEFAULT = ImVec4(0.39f, 0.39f, 0.39f, 1);
ImVec4 NodeColors::Connection::HOVERED = ImVec4(1, 0.43f, 0.35f, 1);
ImVec4 NodeColors::Connection::TRUE = NodeColors::Lookup::GREEN;
ImVec4 NodeColors::Connection::FALSE = NodeColors::Lookup::RED;
ImVec4 NodeColors::Connection::RUNTIME = NodeColors::Lookup::YELLOW;

std::map<std::string, ImVec4> NodeColors::m_kind
{
	{"VARIABLE", NodeColors::Lookup::BLUE},
	{"OPERATION", NodeColors::Lookup::YELLOW},
	{"CAST", NodeColors::Lookup::LIGHT_BLUE},
	{"COMPARISON", NodeColors::Lookup::PINK},
	{"BRANCH", NodeColors::Lookup::GREEN},
	{"ACTION", NodeColors::Lookup::ORANGE},
	{"MATH", NodeColors::Lookup::VIOLET},
};
}
