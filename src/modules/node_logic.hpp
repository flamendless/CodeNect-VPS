#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "modules/node.hpp"
#include "modules/node_var.hpp"
#include "modules/connection.hpp"

namespace CodeNect
{
namespace NodeLogic
{
void change_value(NodeVariable* in, NodeVariable* out);
void on_connect(Node* in, Node* out);
}
}

#endif //_NODE_LOGIC_HPP
