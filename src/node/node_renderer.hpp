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
#include "node/node_ds.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_get.hpp"
#include "node/node_size.hpp"
#include "node/node_string.hpp"
#include "node/node_loop.hpp"
#include "node/node_for.hpp"
#include "node/node_set.hpp"

namespace CodeNect::NodeRenderer
{
void draw_node(Node* node);

void draw_node_val(NodeValue*);
void draw_node_var(NodeVariable*);
void draw_node_op(NodeOperation*);
void draw_node_cast(NodeCast*);
void draw_node_cmp(NodeComparison*);
void draw_node_branch(NodeBranch*);
void draw_node_print(NodePrint*);
void draw_node_prompt(NodePrompt*);
void draw_node_set(NodeSet*);
void draw_node_array_access(NodeArrayAccess*);
void draw_node_math(NodeMath*);
void draw_node_array(NodeArray*);
void draw_node_size(NodeSize*);
void draw_node_string(NodeString*);
void draw_node_for(NodeFor*);

void draw_connections(Node&);
void draw_connected_values(Node*);

void push_node_style(void);
void pop_node_style(void);
void push_node_color(Node*);
void pop_node_color(void);
}

#endif //_NODE_RENDERER_HPP
