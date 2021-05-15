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
	PLOGW << "Current state: " << &state;
	PLOGW << "is_in_else: " << state.is_in_else;
	if (state.node_branch)
	{
		PLOGW << "Has else: " << state.node_branch->m_has_else;
		if (state.is_in_else)
			PLOGW << "On Branch-else: " << state.node_branch->m_name;
		else
			PLOGW << "On Branch-if: " << state.node_branch->m_name;
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

void state_stack_push(State& state)
{
	PLOGW << "---PUSHED---";
	if (state.node_branch)
		PLOGW << state.node_branch->m_name;
	PLOGW << "rest:";
	for (Node* &node : state.v_rest)
		PLOGW << "\t" << node->m_name;
	PLOGW << "is_in_else: " << state.is_in_else;
	PLOGW << "------------";
	Transpiler::v_states.push_back(state);
}

State state_stack_pop()
{
	State state = Transpiler::v_states.back();
	Transpiler::v_states.pop_back();
	PLOGW << "---POPPED---";
	if (state.node_branch)
		PLOGW << state.node_branch->m_name;
	PLOGW << "rest:";
	for (Node* &node : state.v_rest)
		PLOGW << "\t" << node->m_name;
	PLOGW << "is_in_else: " << state.is_in_else;
	PLOGW << "------------";
	return state;
}

bool check_change_block(State& cur_state, State& new_state)
{
	if (!new_state.node_branch)
		return false;
	PLOGD << "Checking Branch Else: " << new_state.node_branch->m_name;
	//switch to else
	if (new_state.node_branch->m_has_else)
	{
		bool found = std::find(Transpiler::v_finished_branches.begin(), Transpiler::v_finished_branches.end(), new_state.node_branch)
			!= Transpiler::v_finished_branches.end();
		if (!found)
		{
			cur_state = new_state;
			cur_state.v_rest.clear();
			cur_state.v_rest.push_back(new_state.node_branch);
			cur_state.is_in_else = true;
			PLOGD << "Going to Branch Else";
			return true;
		}
	}

	//if no else or else is done
	cur_state = state_stack_pop();
	return true;
}

bool switch_to_branch(NodeBranch* node_branch, State& cur_state, State& new_state)
{
	if (new_state.node_branch && new_state.is_in_else)
	{
		if (new_state.node_branch == node_branch)
		{
			Transpiler::v_finished_branches.push_back(node_branch);
			return false;
		}
	}

	if (!new_state.node_branch ||
		(new_state.node_branch && new_state.node_branch != node_branch))
	{
		//we are going/nesting to a new branch
		state_stack_push(new_state);

		//set the state for the next iteration
		State next_state;
		next_state.v_rest.push_back(node_branch);
		next_state.node_branch = node_branch;
		cur_state = next_state;
	}

	return true;
}

int transpile_loop(State& start_state, std::string& str_next)
{
	int pass = 1;
	Transpiler::v_states.clear();
	Transpiler::v_finished_branches.clear();
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
			if (check_change_block(cur_state, new_state))
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
				should_branch_out = switch_to_branch(node_branch, cur_state, new_state);
			}
			else
			{
				if (v.size() == 0)
					continue;
				// PLOGD << "storing to stack the rest...";
				// State state;
				// state.v_rest = cur_state.v_rest;
				// state.is_in_else = cur_state.is_in_else;
				// v_states.push_back(state);
			}
		}

		if (should_branch_out)
			continue;

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

Node* Transpiler::transpile(std::vector<Node*>& v, std::string& output, State* current_state)
{
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;
		bool found = Transpiler::m_declared.find(node->m_name) != Transpiler::m_declared.end();
		if (found)
		{
			if (current_state && current_state->node_branch && current_state->is_in_else)
			{
				NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
				if (!node_branch)
					continue;
			}
			else
			{
				NodeArray* node_array = dynamic_cast<NodeArray*>(node);
				if (!node_array)
				{
					PLOGW << node->m_name << " is declared already. Skipping.";
					continue;
				}
			}
		}
		else
		{
			Transpiler::m_declared.insert({node->m_name, true});
			++Transpiler::n_transpiled;
		}

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = static_cast<NodeVariable*>(node);
				output.append(NodeToCode::comment(node));
				output.append(NodeToCode::ntc_var(node_var));
				output.append("\n");
				break;
			}

			case NODE_KIND::DS:
			{
				NodeDS* node_ds = static_cast<NodeDS*>(node);
				switch (node_ds->m_ds)
				{
					case NODE_DS::EMPTY: break;
					case NODE_DS::ARRAY:
					{
						NodeArray* node_array = static_cast<NodeArray*>(node);
						bool found = Transpiler::m_array_init.find(node_array->m_name) != Transpiler::m_array_init.end();
						if (found)
							continue;
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_array(node_array));
						output.append("\n");
						break;
					}
				}
				break;
			}

			case NODE_KIND::ACTION:
			{
				NodeAction* node_action = static_cast<NodeAction*>(node);
				switch (node_action->m_action)
				{
					case NODE_ACTION::EMPTY: break;
					case NODE_ACTION::PRINT:
					{
						NodePrint* node_print = static_cast<NodePrint*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_print(node_print));
						output.append("\n");
						break;
					}
					case NODE_ACTION::PROMPT:
					{
						NodePrompt* node_prompt = static_cast<NodePrompt*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_prompt(node_prompt));
						output.append("\n");
						break;
					}
					case NODE_ACTION::SET:
					{
						NodeSet* node_set = static_cast<NodeSet*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_set(node_set));
						output.append("\n");
						break;
					}
				}
				break;
			}

			case NODE_KIND::CAST: break;
			case NODE_KIND::OPERATION: break;
			case NODE_KIND::COMPARISON: break;
			case NODE_KIND::GET:
			{
				NodeGet* node_get = static_cast<NodeGet*>(node);
				switch (node_get->m_get)
				{
					case NODE_GET::EMPTY: break;
					case NODE_GET::SIZE:
					{
						NodeSize* node_size = static_cast<NodeSize*>(node_get);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_size(node_size, false, output));
						output.append("\n");
						break;
					}
					case NODE_GET::ARRAY_ACCESS:
					{
						NodeArrayAccess* node_array_access = static_cast<NodeArrayAccess*>(node_get);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_array_access(node_array_access, false, output));
						output.append("\n");
						break;
					}
				}
				break;
			}

			case NODE_KIND::BRANCH:
			{
				bool found = std::find(Transpiler::v_finished_branches.begin(), Transpiler::v_finished_branches.end(), current_state->node_branch)
					!= Transpiler::v_finished_branches.end();
				bool is_else = false;

				if (current_state->is_in_else && !found)
					is_else = true;

				NodeBranch* node_branch = static_cast<NodeBranch*>(node);
				output.append(NodeToCode::comment(node));
				output.append(NodeToCode::ntc_branch(node_branch, is_else));
				output.append(NodeToCode::indent()).append("{").append("\n");
				Transpiler::level++;
				output.append("\n");
				return node_branch;
				break;
			}

			case NODE_KIND::STRING:
			{
				NodeString* node_str = static_cast<NodeString*>(node);
				output.append(NodeToCode::comment(node));
				output.append(NodeToCode::ntc_string(node_str, false, output));
				output.append("\n");
				break;
			}

			case NODE_KIND::LOOP:
			{
				NodeLoop* node_loop = static_cast<NodeLoop*>(node);
				switch (node_loop->m_loop)
				{
					case NODE_LOOP::EMPTY: break;
					case NODE_LOOP::FOR:
					{
						NodeFor* node_for = static_cast<NodeFor*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_for(node_for));
						output.append(NodeToCode::indent()).append("{").append("\n");
						output.append("\n");
						Transpiler::level++;
						output.append("\n");
						return node_for;
					}
					case NODE_LOOP::WHILE: break;
				}
			}
		}
	}
	return nullptr;
}
}
