#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "modules/node.hpp"
#include "modules/node_var.hpp"
#include "modules/node_op.hpp"
#include "modules/connection.hpp"

namespace CodeNect
{
namespace NodeLogic
{
void process_var(NodeVariable* node);
void perform_op(NodeVariable* node, NodeOp* op);
void op_add(NodeVariable* node, NodeOp* op);
void process();
}
}

#endif //_NODE_LOGIC_HPP
