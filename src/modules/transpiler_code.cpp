#include "modules/transpiler.hpp"

#include "plog/Log.h"
#include "ppk_assert.h"
#include "fmt/format.h"
#include "modules/templates.hpp"
#include "modules/node_to_code.hpp"
#include "node/nodes.hpp"
#include "core/project.hpp"
#include "core/defines.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_math.hpp"
#include "node/node_array.hpp"
#include "node/node_var.hpp"
#include "node/node_cast.hpp"
#include "node/node_op.hpp"
#include "node/node_array_access.hpp"
#include "node/node_branch.hpp"
#include "node/node_loop.hpp"
#include "node/node_for.hpp"

namespace CodeNect
{
void get_v_decls(std::vector<Node*>& v_decls, std::vector<Node*>& v_decls_array)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = *it;
		if (Nodes::check_if_no_lhs(node))
		{
			if (!Transpiler::is_valid_decls(node))
			{
				NodeArray* node_array = dynamic_cast<NodeArray*>(node);
				if (!node_array)
				{
					Transpiler::add_message(std::move("Node is not a valid declaration because it needs an inputs"),
							OUTPUT_TYPE::WARNING, node, DOC_ID::NEED_INPUTS);
				}
			}
			else
				v_decls.push_back(node);
		}

		//check if inputs are satisfied
		NodeOperation* node_op = dynamic_cast<NodeOperation*>(node);
		if (node_op)
		{
			const int count = Nodes::count_node_dep(node_op);
			if (count < 2)
				Transpiler::add_message(std::move("NodeOperation must have at least 2 inputs"),
						OUTPUT_TYPE::WARNING, node_op, DOC_ID::OP_REQ);
		}

		//exception: still define all arrays
		NodeArray* node_array = dynamic_cast<NodeArray*>(node);
		if (node_array)
			v_decls_array.push_back(node);
	}
}

void print_state(State& state)
{
	PLOGD << "state: " << &state;
	if (state.node_branch)
	{
		if (state.is_in_else)
			PLOGD << "On Branch-else: " << state.node_branch->m_name;
		else
			PLOGD << "On Branch-if: " << state.node_branch->m_name;
	}
}

void print_vec(std::vector<Node*>& v, std::string str)
{
	if (v.size() == 0)
		return;
	PLOGD << str;
	PLOGD << "{";
	for (Node* &node : v)
		PLOGD << "\t" << node->m_name;
	PLOGD << "}";
}

void print_vec(std::vector<std::vector<Node*>>& v, std::string str)
{
	if (v.size() == 0)
		return;
	PLOGD << str;
	for (std::vector<Node*>& v : v)
	{
		if (v.size() == 0)
			return;

		PLOGD << "{";
		for (Node* &node : v)
			PLOGD << "\t" << node->m_name;
		PLOGD << "}";
	}
}

bool check_change_block(State& cur_state, State& new_state, std::vector<State>& v_states)
{
	if (!new_state.node_branch)
		return false;
	PLOGD << "Checking Branch Else: " << new_state.node_branch->m_name;
	if (new_state.node_branch->m_has_else && !new_state.is_in_else)
	{
		PLOGD << "Going to else-statement...";
		cur_state.v_rest.push_back(new_state.node_branch);
		cur_state.is_in_else = true;
	}
	else
	{
		PLOGD << "ending branch: " << new_state.node_branch->m_name;
		cur_state = v_states.back();
		v_states.pop_back();
	}
	return true;
}

State switch_to_branch(NodeBranch* node_branch, State& new_state, std::vector<State>& v_states)
{
	v_states.push_back(new_state);
	State branch_state;
	branch_state.node_branch = node_branch;
	branch_state.v_rest.push_back(node_branch);

	return branch_state;
}

int transpile_loop(State& start_state, std::string& str_next)
{
	int pass = 1;
	std::vector<State> v_states;
	State cur_state = start_state;
	bool is_loop = true;

	while (is_loop)
	{
		PLOGD << "PASS #" << pass;
		print_state(cur_state);
		State new_state = cur_state;

		//ENTRY
		print_vec(new_state.v_rest, "entry");

		//SEQUENCE
		new_state.v_seq = Transpiler::get_v_sequence(&new_state);
		for (std::vector<Node*>& v : new_state.v_seq)
			Transpiler::arrange_v(v);
		print_vec(new_state.v_seq, "sequence");

		//REST
		new_state.v_rest = Transpiler::get_rest(&new_state);
		Transpiler::arrange_v(new_state.v_rest);
		print_vec(new_state.v_rest, "rest");

		//handle changing branch
		if (new_state.v_seq.size() == 0 && new_state.v_rest.size() == 0)
		{
			if (check_change_block(cur_state, new_state, v_states))
			{
				Transpiler::level--;
				str_next.append(NodeToCode::indent()).append("}").append("\n");
				continue;
			}
			else
				break;
		}

		PLOGD << "Transpiling sequence...";
		//handle when we enter a branch
		bool should_branch_out = false;
		State branch_state;
		for (std::vector<Node*>& v : new_state.v_seq)
		{
			if (!should_branch_out)
			{
				Node* node = Transpiler::transpile(v, str_next, &new_state);
				if (!node)
					continue;
				NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
				if (!node_branch)
					continue;

				if (!new_state.node_branch ||
					(new_state.node_branch && new_state.node_branch != node_branch))
				{
					branch_state = switch_to_branch(node_branch, new_state, v_states);
					should_branch_out = true;
				}
			}
			else
			{
				if (v.size() == 0)
					continue;
				PLOGD << "storing to stack the rest...";
				State state;
				state.v_rest = cur_state.v_rest;
				state.is_in_else = cur_state.is_in_else;
				v_states.push_back(state);
			}
		}

		if (should_branch_out)
		{
			// set for the branch
			cur_state = branch_state;
			continue;
		}

		PLOGD << "Transpiled sequence";
		PLOGD << "Transpiling rest...";
		Transpiler::transpile(new_state.v_rest, str_next);
		PLOGD << "Transpiled rest";

		//set for the next loop
		cur_state = new_state;
		++pass;
	}
	return pass;
}

void Transpiler::build_runnable_code(std::string& out, bool is_tcc)
{
	Transpiler::n_transpiled = 0;
	Transpiler::v_declarations.clear();
	Transpiler::m_temp_names.clear();
	Transpiler::m_declared.clear();
	Transpiler::m_array_init.clear();

	std::string str_incl = "";
	std::string str_structs = "";
	std::string str_entry = "";
	std::string str_decls = "";
	std::string str_next = "";
	std::string str_closing = "";
	std::string str_free = "";

	Transpiler::m_temp_names.clear();
	Transpiler::set_pre_entry(str_incl, str_structs, is_tcc);

	//entry point
	str_entry.append("int main()").append("\n");
	str_entry.append("{").append("\n");

	//find all nodes that do NOT have any LHS, this means that they are for declarations
	std::vector<Node*> v_decls;
	std::vector<Node*> v_decls_array;
	get_v_decls(v_decls, v_decls_array);

	//begin transpiling
	Transpiler::level++;

	PLOGD << "Transpiling decls... size = " << v_decls.size();
	Transpiler::transpile_decls(v_decls, str_decls);
	PLOGD << "Transpiled decls";

	PLOGD << "Transpiling decls array... size = " << v_decls_array.size();
	Transpiler::transpile_decls_array(v_decls_array, str_decls);
	PLOGD << "Transpiled decls array";

	State start_state;
	start_state.v_rest = v_decls;
	start_state.v_rest.insert(start_state.v_rest.end(), v_decls_array.begin(), v_decls_array.end());
	int passes = transpile_loop(start_state, str_next);
	PLOGD << "Total passes: " << passes;

	if (is_tcc)
	{
		std::string str_transpiled_nodes = fmt::format("Transpiled Nodes: {:d}/{:d}",
				Transpiler::n_transpiled, Nodes::v_nodes.size());
		std::string str_pass = fmt::format("Total Passes: {:d}", passes);
		Transpiler::add_message(std::move(str_transpiled_nodes));
		Transpiler::add_message(std::move(str_pass));
	}

	//get structs to be freed
	for (std::pair<const std::string, std::string>& e : Transpiler::m_array_init)
	{
		std::string str = fmt::format("  {:s}(&{:s});", e.second, e.first);
		str_free.append(str).append("\n");
	}
	Transpiler::level--;

	//closing
	str_closing.append("  printf(\"PRESS ENTER TO EXIT\\n\");").append("\n");
	str_closing.append("  getchar();").append("\n");
	str_closing.append("  return 0;").append("\n");
	str_closing.append("}");

	out.append(fmt::format("//Project: {:s}\n", Project::meta.title))
		.append(fmt::format("//Author: {:s}\n", Project::meta.author))
		.append("//C code generated by CodeNect\n\n")
		.append(Templates::s_incl_section)
		.append(str_incl)
		.append(Templates::e_incl_section)

		.append(Templates::s_structs_section)
		.append(str_structs).append("\n")
		.append(Templates::s_structs_section)

		.append(str_entry)
		.append(str_decls).append("\n")
		.append(str_next).append("\n")
		.append(str_free).append("\n")
		.append(str_closing);
	Transpiler::m_temp_names.clear();
}
}
