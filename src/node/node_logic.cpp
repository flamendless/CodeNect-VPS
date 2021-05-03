#include "node/node_logic.hpp"
#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_branch.hpp"
#include "node/nodes.hpp"
#include "node/node_colors.hpp"
#include "modules/debugger.hpp"

namespace CodeNect::NodeLogic
{
std::vector<Node*> traverse_node(Node* node)
{
	std::vector<Node*> v_out;
	for (const Connection& connection : node->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node)
			continue;

		NodeBranch* node_branch = dynamic_cast<NodeBranch*>(out_node);
		if (node_branch)
			v_out.push_back(node_branch);

		std::vector<Node*> v_temp = NodeLogic::traverse_node(out_node);
		for (Node* &node : v_temp)
		{
			bool found = std::find(v_out.begin(), v_out.end(), node) != v_out.end();
			if (!found)
				v_out.push_back(node);
		}
	}
	return v_out;
}

std::vector<std::vector<Node*>> get_branch_path(Node* node)
{
	std::vector<std::vector<Node*>> v_out;
	for (const Connection& connection : node->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node)
			continue;

		std::vector<Node*> v_temp = NodeLogic::traverse_node(node);
		if (v_temp.size() != 0)
			v_out.push_back(v_temp);
	}
	return v_out;
}

bool valid_branch_path(std::vector<std::vector<Node*>>& v_branch_seq)
{
	bool is_valid = true;
	std::vector<Node*>& v_main_path = v_branch_seq[0];
	Node* first_branch = v_main_path[0];
	if (v_main_path.size() > 1)
	{
		std::vector<std::vector<Node*>> v_sub_path = NodeLogic::get_branch_path(first_branch);
		if (v_sub_path.size() == 0)
			return false;
		else
		{
			is_valid = NodeLogic::valid_branch_path(v_sub_path);
			if (v_sub_path[0].size() != v_main_path.size() - 1)
				is_valid = false;
			else
			{
				bool same = std::equal(v_sub_path[0].begin(), v_sub_path[0].end(), v_main_path.begin() + 1);
				if (!same)
					is_valid = false;
			}
		}
	}
	return is_valid;
}

void validate_branches(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = static_cast<Node*>(*it);
		const int dep_count = Nodes::count_node_dep(node);
		if (dep_count == 0)
			continue;
		std::vector<std::vector<Node*>> v_branch_seq = NodeLogic::get_branch_path(node);
		if (v_branch_seq.size() > 1)
			if (!NodeLogic::valid_branch_path(v_branch_seq))
				NodeLogic::invalid_branch_colorize(node);
	}
}

void invalid_branch_colorize(Node* node)
{
	for (Connection& connection : node->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node)
			continue;

		NodeColors::set_connection_color(connection, COLOR_TYPE::FALSE);
		Debugger::add_message(std::move("Conflict with branches"), OUTPUT_TYPE::WARNING, node);
	}
}

void process(void)
{
	Debugger::clear();

	//reset colors of connections
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = static_cast<Node*>(*it);
		for (Connection& connection : node->m_connections)
			NodeColors::set_connection_color(connection, COLOR_TYPE::DEFAULT);
	}

	NodeLogic::process_var();
	NodeLogic::process_math();
	NodeLogic::process_op();
	NodeLogic::process_cast();
	NodeLogic::process_array();
	NodeLogic::process_array_access();
	NodeLogic::process_size();
	NodeLogic::process_cmp();
	NodeLogic::process_branch();
	NodeLogic::process_print();
	NodeLogic::process_prompt();

	NodeLogic::validate_branches();
}
}
