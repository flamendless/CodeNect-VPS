#ifndef _NODE_TO_CODE_HPP
#define _NODE_TO_CODE_HPP

#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"

namespace CodeNect::NodeToCode
{
std::string indent(void);
std::string slot_to_str(NODE_SLOT&);
std::string slot_to_spec(NODE_SLOT&);
std::string to_array(NodeArray*);
std::string comment(Node*);

std::string node_var(NodeVariable*);
std::string node_cast(NodeCast*, bool, std::string&);
std::string node_array(NodeArray*);
std::string node_print(NodePrint*);
std::string node_prompt(NodePrompt*);
std::string node_op(NodeOperation*, bool, std::string&);
}

#endif //_NODE_TO_CODE_HPP
