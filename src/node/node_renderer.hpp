#ifndef _NODE_RENDERER_HPP
#define _NODE_RENDERER_HPP

#include "node/node.hpp"
#include "node/node_var.hpp"
#include "node/node_val.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"
#include "node/node_action.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_math.hpp"

namespace CodeNect::NodeRenderer
{
void draw_node(Node* node);

void draw_node_val(NodeValue* node_val);
void draw_node_var(NodeVariable* node_var);
void draw_node_op(NodeOperation* node_op);
void draw_node_cast(NodeCast* node_cast);
void draw_node_cmp(NodeComparison* node_cmp);
void draw_node_branch(NodeBranch* node_branch);
void draw_node_print(NodePrint* node_print);
void draw_node_prompt(NodePrompt* node_prompt);
void draw_node_math(NodeMath* node_math);

void draw_connections(Node& node);
void draw_connected_values(Node* node);

void push_node_style(void);
void pop_node_style(void);
void push_node_color(Node* node);
void pop_node_color(void);
}

#endif //_NODE_RENDERER_HPP
