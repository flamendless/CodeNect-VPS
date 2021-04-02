#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_math.hpp"

namespace CodeNect::NodeLogic
{
void process(void);
void process_var(void);
void process_op(void);
void process_cast(void);
void process_cmp(void);
void process_branch(void);
void process_print(void);
void process_math(void);
void process_math_root(NodeMath* node_math);
bool validate_node_cast(Node* in_node, Node* out_node);
}

#endif //_NODE_LOGIC_HPP
