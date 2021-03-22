#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "modules/node.hpp"
#include "modules/node_var.hpp"
#include "modules/node_op.hpp"
#include "modules/connection.hpp"

namespace CodeNect::NodeLogic
{
void process(void);
void process_var(void);
void process_op(void);
}

#endif //_NODE_LOGIC_HPP
