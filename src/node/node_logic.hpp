#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_cmp.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_array_access.hpp"
#include "node/node_array.hpp"
#include "node/node_math.hpp"
#include "node/node_branch.hpp"
#include "node/node_size.hpp"
#include "node/node_string.hpp"
#include "ui/alert.hpp"
#include "ui/docs.hpp"
#include "modules/simulation.hpp"

namespace CodeNect::NodeLogic
{
void process(void);
void process_node(Node*);
void process_var(NodeVariable*);
void process_op(NodeOperation*);
void process_cast(NodeCast*);
void process_cmp(NodeComparison*);
void process_branch(NodeBranch*);
void process_print(NodePrint*);
void process_prompt(NodePrompt*);
void process_math(NodeMath*);
double calculate_math(NodeMath* node_math, std::function<double(double, double)> fn);
void process_array(NodeArray*);
bool index_within_array_bounds(NodeArray*, const int);
void process_array_access(NodeArrayAccess*);
void process_size(NodeSize*);
void process_string(NodeString*);
void process_for(NodeFor*);

bool check_cyclic(Node* in_node, Node* out_node);
bool validate_node_print(Node* in_node, Node* out_node);
bool validate_node_array(Node* in_node, Node* out_node);
bool validate_node_string(Node* in_node, Node* out_node);
bool validate_node_for(Node* in_node, Node* out_node);

//NodeBranch
struct BranchInfo
{
	NodeBranch* node_branch = nullptr;
	bool is_else = false;

	bool operator==(const BranchInfo& other) const
	{
		return node_branch == other.node_branch
			&& is_else == other.is_else;
	}
	bool operator!=(const BranchInfo& other) const
	{
		return !operator ==(other);
	}
};

std::vector<std::vector<BranchInfo>> get_branch_path(Node*);
std::vector<BranchInfo> traverse_node(Node*);
bool valid_branch_path(std::vector<std::vector<BranchInfo>>&);
void invalid_branch_colorize(Node*);
void validate_branches(void);
}

#endif //_NODE_LOGIC_HPP
