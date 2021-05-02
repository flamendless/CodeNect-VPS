#ifndef _NODE_TO_CODE_HPP
#define _NODE_TO_CODE_HPP

#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"
#include "node/node_math.hpp"
#include "node/node_cmp.hpp"
#include "node/node_size.hpp"
#include "node/node_array_access.hpp"
#include "node/node_branch.hpp"

namespace CodeNect::NodeToCode
{
std::string indent(void);
std::string slot_to_str(NODE_SLOT&);
std::string slot_to_spec(NODE_SLOT&);
std::string cmp_to_str(NODE_CMP&);
std::string to_array(NodeArray*);
std::string comment(Node*);

std::string ntc_var(NodeVariable*);
std::string ntc_cast(NodeCast*, bool, std::string&);
std::string ntc_array(NodeArray*);
std::string ntc_array_decls(NodeArray*);
std::string ntc_print(NodePrint*);
std::string ntc_prompt(NodePrompt*);
std::string ntc_op(NodeOperation*, bool, std::string&);
std::string ntc_math(NodeMath*, bool, std::string&);
std::string ntc_cmp(NodeComparison*, bool, std::string&);
std::string ntc_size(NodeSize*, bool, std::string&);
std::string ntc_array_access(NodeArrayAccess*, bool, std::string&);
std::string ntc_branch(NodeBranch*);
}

#endif //_NODE_TO_CODE_HPP
