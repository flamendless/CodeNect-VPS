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
BETTER_ENUM(NODE_KIND, int, EMPTY = 1, VARIABLE, OPERATION, CAST, COMPARISON,
	BRANCH, ACTION, MATH, DS, GET, STRING, LOOP)
BETTER_ENUM(NODE_OP, int, EMPTY = 1, ADD, SUB, MUL, DIV, MOD)
BETTER_ENUM(NODE_ACTION, int, EMPTY = 1, PRINT, PROMPT, SET)
BETTER_ENUM(NODE_GET, int, EMPTY = 1, ARRAY_ACCESS, SIZE)
BETTER_ENUM(NODE_LOOP, int, EMPTY = 1, FOR, WHILE)

//CMP
//EQ  - ==
//NEQ - !=
//LT  - <
//GT  - >
//LTE - <=
//GTE - >=
//OR - ||
//AND - &&
BETTER_ENUM(NODE_CMP, int, EMPTY = 1, EQ, NEQ, LT, GT, LTE, GTE, OR, AND)

//MATH
//ROOT - root
//POW - power
//SIN - sine
//COS - cosine
//TAN - tangent
BETTER_ENUM(NODE_MATH, int, EMPTY = 1, ROOT, POW, SIN, COS, TAN)

//STRING
//TOLOWERCASE
//TOUPPERCASE
//TOARRAY
BETTER_ENUM(NODE_STRING, int, EMPTY = 1, TLC, TUC, REVERSE, TOARRAY)

//Data Structures
BETTER_ENUM(NODE_DS, int, EMPTY = 1, ARRAY)
BETTER_ENUM(NODE_ARRAY, int, EMPTY = 1, FIXED, DYNAMIC)

typedef std::vector<Connection> v_connection_t;
typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;
}

#endif //_NODE_DEF_HPP
