#ifndef _NODE_TO_CODE_HPP
#define _NODE_TO_CODE_HPP

#include "node/node_var.hpp"
#include "node/node_array.hpp"

namespace CodeNect::NodeToCode
{
std::string slot_to_str(NODE_SLOT&);
std::string to_fixed_array(NodeArray*);
std::string to_dynamic_array(NodeArray*, const std::string&);
std::string comment(Node*);
std::string node_var(NodeVariable*);
std::string node_array(NodeArray*);
}

#endif //_NODE_TO_CODE_HPP
