#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"

namespace CodeNect::NodeLogic
{
void process(void);
void process_var(void);
void process_op(void);
void process_cast(void);
}

#endif //_NODE_LOGIC_HPP
