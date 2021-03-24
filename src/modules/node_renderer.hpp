#ifndef _NODE_RENDERER_HPP
#define _NODE_RENDERER_HPP

#include "modules/node.hpp"
#include "modules/node_var.hpp"
#include "modules/node_val.hpp"
#include "modules/node_op.hpp"

namespace CodeNect::NodeRenderer
{
void draw_node(Node* node);
void draw_node_var(NodeVariable* node_var);
void draw_node_val(NodeValue* node_val);
void draw_node_op(NodeOperation* node_op);
void draw_connections(Node& node);
void draw_connected(Node* node);
}

#endif //_NODE_RENDERER_HPP
