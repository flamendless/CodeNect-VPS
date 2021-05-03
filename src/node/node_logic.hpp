#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_math.hpp"
#include "node/node_array.hpp"

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
bool index_within_array_bounds(NodeArray*, const int);
void process_array_access(void);
void process_size(void);

bool validate_node_print(Node* in_node, Node* out_node);

std::vector<std::vector<Node*>> get_branch_path(Node*);
std::vector<Node*> traverse_node(Node*);
bool valid_branch_path(std::vector<std::vector<Node*>>&);
void invalid_branch_colorize(Node*);
void validate_branches(void);
}

#endif //_NODE_LOGIC_HPP
