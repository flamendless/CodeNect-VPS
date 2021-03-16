#ifndef _NODE_DEF_HPP
#define _NODE_DEF_HPP

#include <vector>
#include "ImNodesEz.h"
#include "enum.h"
#include "modules/nodes_connection.hpp"

namespace CodeNect
{
BETTER_ENUM(NODE_SLOT, char, EMPTY = 1, BOOL, INTEGER, FLOAT, DOUBLE, STRING)
BETTER_ENUM(NODE_KIND, char, EMPTY = 1, VARIABLE, OPERATION)
BETTER_ENUM(NODE_OP, char, EMPTY = 1, ADD, SUB, MUL, DIV)

typedef std::vector<Connection> v_connection_t;
typedef std::vector<ImNodes::Ez::SlotInfo> v_slot_info_t;
}

#endif //_NODE_DEF_HPP
