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
BETTER_ENUM(NODE_KIND, int, EMPTY = 1, VARIABLE, OPERATION, CAST, COMPARISON, BRANCH, ACTION)
BETTER_ENUM(NODE_OP, int, EMPTY = 1, ADD, SUB, MUL, DIV)
BETTER_ENUM(NODE_ACTION, int, EMPTY = 1, PRINT)

//EQ  - ==
//NEQ - !=
//LT  - <
//GT  - >
//LTE - <=
//GTE - >=
//OR - ||
//AND - &&
BETTER_ENUM(NODE_CMP, int, EMPTY = 1, EQ, NEQ, LT, GT, LTE, GTE, OR, AND)

typedef std::vector<Connection> v_connection_t;
typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;
}

#endif //_NODE_DEF_HPP
