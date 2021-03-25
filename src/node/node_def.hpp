#ifndef _NODE_DEF_HPP
#define _NODE_DEF_HPP

#include <vector>
#include "plog/Log.h"
#include "ImNodesEz.h"
#include "enum.h"
#include "node/connection.hpp"

namespace CodeNect
{
BETTER_ENUM(NODE_SLOT, int, EMPTY = 1, BOOL, INTEGER, FLOAT, DOUBLE, STRING)
BETTER_ENUM(NODE_KIND, int, EMPTY = 1, VARIABLE, OPERATION, CAST, IF)
BETTER_ENUM(NODE_OP, int, EMPTY = 1, ADD, SUB, MUL, DIV)

typedef std::vector<Connection> v_connection_t;
typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;
}

#endif //_NODE_DEF_HPP
