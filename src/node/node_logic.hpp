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
void process_prompt(void);
void process_math(void);
double calculate_math(NodeMath* node_math, std::function<double(double, double)> fn);
void process_array(void);
void process_array_access(void);
void process_size(void);

bool validate_node_entry(Node* in_node, Node* out_node);
bool validate_node_array_access(Node* in_node, Node* out_node);
}

#endif //_NODE_LOGIC_HPP
