#ifndef _NODE_RENDERER_HPP
#define _NODE_RENDERER_HPP

#include "node/node.hpp"
#include "node/node_var.hpp"
#include "node/node_val.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_cmp.hpp"

namespace CodeNect::NodeRenderer
{
void draw_node(Node* node);
void draw_node_var(NodeVariable* node_var);
void draw_node_val(NodeValue* node_val);
void draw_node_op(NodeOperation* node_op);
void draw_node_cast(NodeCast* node_cast);
void draw_node_cmp(NodeComparison* node_cmp);
void draw_connections(Node& node);
void draw_connected_op(NodeOperation* node_op);
void draw_connected_cmp(NodeComparison* node_cmp);
}

#endif //_NODE_RENDERER_HPP
