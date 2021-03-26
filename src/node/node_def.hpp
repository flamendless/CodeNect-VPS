#ifndef _NODE_DEF_HPP
#define _NODE_DEF_HPP

#include <map>
#include <vector>
#include "plog/Log.h"
#include "IconsFontAwesome5.h"
#include "ImNodesEz.h"
#include "enum.h"
#include "node/connection.hpp"

namespace CodeNect
{
BETTER_ENUM(NODE_SLOT, int, EMPTY = 1, BOOL, INTEGER, FLOAT, DOUBLE, STRING)
BETTER_ENUM(NODE_KIND, int, EMPTY = 1, VARIABLE, OPERATION, CAST, IF, COMPARISON)
BETTER_ENUM(NODE_OP, int, EMPTY = 1, ADD, SUB, MUL, DIV)

//EQ  - ==
//NEQ - !=
//LT  - <
//GT  - >
//LTE - <=
//GTE - >=
BETTER_ENUM(NODE_CMP, int, EMPTY = 1, EQ, NEQ, LT, GT, LTE, GTE)

inline std::map<std::string, std::pair<const char*, const char*>> m_cmp_str
{
	{"EQ", std::make_pair<const char*, const char*>("equal", ICON_FA_EQUALS)},
	{"NEQ", std::make_pair<const char*, const char*>("not equal", ICON_FA_NOT_EQUAL)},
	{"LT", std::make_pair<const char*, const char*>("less than", ICON_FA_LESS_THAN)},
	{"GT", std::make_pair<const char*, const char*>("greater than", ICON_FA_GREATER_THAN)},
	{"LTE", std::make_pair<const char*, const char*>("less than or equals to", ICON_FA_LESS_THAN_EQUAL)},
	{"GTE", std::make_pair<const char*, const char*>("greater than or equals to", ICON_FA_GREATER_THAN_EQUAL)},
};

typedef std::vector<Connection> v_connection_t;
typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;
}

#endif //_NODE_DEF_HPP
