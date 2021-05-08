#ifndef _NODE_LOGIC_HPP
#define _NODE_LOGIC_HPP

#include "node/connection.hpp"
#include "node/node.hpp"
#include "node/node_math.hpp"
#include "node/node_array.hpp"
#include "node/node_branch.hpp"
#include "ui/alert.hpp"
#include "ui/docs.hpp"

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
void process_string(void);

bool validate_node_print(Node* in_node, Node* out_node);
bool validate_node_array(Node* in_node, Node* out_node);
bool validate_node_string(Node* in_node, Node* out_node);
bool validate_node_for(Node* in_node, Node* out_node);

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
