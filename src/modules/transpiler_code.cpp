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
#include "node/node_for.hpp"

namespace CodeNect
{
std::vector<NodeVariable*> v_iterators;
std::vector<Node*> v_deferred;
std::string* str_so_far;

Node* find_it_node(NodeFor* node_for)
{
	for (const Connection& connection : node_for->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node != node_for)
			continue;
		const char* slot = connection.out_slot;
		if (std::strcmp(slot, "INTEGER - iterator") == 0)
		{
			Node* in_node = static_cast<Node*>(connection.in_node);
			return in_node;
		}
	}
	return nullptr;
}

void get_v_decls(std::vector<Node*>& v_decls, std::vector<Node*>& v_decls_array, std::vector<Node*>& v_deferred)
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
			{
				NodeFor* node_for = dynamic_cast<NodeFor*>(node);
				if (node_for)
					v_deferred.push_back(node);
				else
					v_decls.push_back(node);
			}
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

void print_state(State* state)
{
	PLOGW << "Current state: " << state;
	if (state->node_branch)
	{
		PLOGW << "Has else: " << state->node_branch->m_has_else;
		PLOGW << "is_in_else: " << state->is_in_else;
		if (state->is_in_else)
			PLOGW << "On Branch-else: " << state->node_branch->m_name;
		else
			PLOGW << "On Branch-if: " << state->node_branch->m_name;
	}
	if (state->node_for)
	{
		PLOGW << "On For: " << state->node_for->m_name;
	}
}

void print_vec(std::vector<Node*>& v, std::string str)
{
	PLOGD << str;
	if (v.size() == 0)
		return;
	PLOGD << "{";
	for (Node* &node : v)
		PLOGD << "\t" << node->m_name;
	PLOGD << "}";
	PLOGD << "------";
}

void print_vec(std::vector<std::vector<Node*>>& v, std::string str)
{
	PLOGD << str;
	if (v.size() == 0)
		return;
	for (std::vector<Node*>& v : v)
	{
		if (v.size() == 0)
			return;

		PLOGD << "{";
		for (Node* &node : v)
			PLOGD << "\t" << node->m_name;
		PLOGD << "}";
	}
	PLOGD << "------";
}

void state_stack_push(State& state)
{
	PLOGW << "---PUSHED---";
	if (state.node_branch)
		PLOGW << "nb: " << state.node_branch->m_name;
	if (state.node_for)
		PLOGW << "nf: " << state.node_for->m_name;
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
		PLOGW << "nb: " << state.node_branch->m_name;
	if (state.node_for)
		PLOGW << "nf: " << state.node_for->m_name;
	PLOGW << "rest:";
	for (Node* &node : state.v_rest)
		PLOGW << "\t" << node->m_name;
	PLOGW << "is_in_else: " << state.is_in_else;
	PLOGW << "------------";
	return state;
}

void get_iterator_nodes(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = *it;
		NodeVariable* node_var = dynamic_cast<NodeVariable*>(node);
		if (!node_var)
			continue;
		for (const Connection& connection : node->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			Node* in_node = static_cast<Node*>(connection.in_node);
			NodeFor* out_node_for = dynamic_cast<NodeFor*>(out_node);
			if (out_node_for && in_node == node)
			{
				const char* slot = connection.out_slot;
				if (std::strcmp(slot, "INTEGER - iterator") == 0)
				{
					if (std::strcmp(out_node_for->m_iterator_name.c_str(), in_node->m_name) == 0)
						v_iterators.push_back(node_var);
				}
			}
		}
	}
	PLOGW << "iterator variables:";
	for (NodeVariable* &node : v_iterators)
		PLOGW << "\t" << node->m_name;
}

bool check_change_block(State& cur_state, State& new_state)
{
	if (!new_state.node_branch)
		return false;
	PLOGD << "Checking Branch Else: " << new_state.node_branch->m_name;
	//switch to else
	if (new_state.node_branch->m_has_else)
	{
		bool found = std::find(Transpiler::v_finished_branches.begin(),
				Transpiler::v_finished_branches.end(), new_state.node_branch)
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

bool check_end_for(State& cur_state, State& new_state)
{
	if (!new_state.node_for)
		return false;
	PLOGD << "Exiting For: " << new_state.node_for->m_name;
	cur_state = state_stack_pop();
	return true;
}

bool switch_to_branch(NodeBranch* node_branch, State& cur_state, State& new_state)
{
	if (new_state.node_branch && new_state.is_in_else &&
		new_state.node_branch == node_branch)
	{
		Transpiler::v_finished_branches.push_back(node_branch);
		return false;
	}

	if (!new_state.node_branch ||
		(new_state.node_branch && new_state.node_branch != node_branch))
	{
		state_stack_push(new_state);
		State next_state;
		next_state.v_rest.push_back(node_branch);
		next_state.node_branch = node_branch;
		cur_state = next_state;
	}
	return true;
}

bool switch_to_for(NodeFor* node_for, State& cur_state, State& new_state)
{
	if (!new_state.node_for ||
		(new_state.node_for && new_state.node_for != node_for))
	{
		state_stack_push(new_state);
		State next_state;
		next_state.v_rest.push_back(find_it_node(node_for));
		next_state.v_rest.push_back(node_for);
		next_state.node_for = node_for;
		cur_state = next_state;
	}
	return true;
}

int transpile_loop(State& start_state, std::string& str_next)
{
	int pass = 1;
	Transpiler::v_states.clear();
	Transpiler::v_finished_branches.clear();
	v_iterators.clear();
	get_iterator_nodes();
	State cur_state = start_state;
	bool is_loop = true;
	str_so_far = &str_next;

	while (is_loop)
	{
		PLOGD << "PASS #" << pass;
		print_state(&cur_state);
		State new_state = cur_state;

		//ENTRY
		print_vec(new_state.v_rest, "entry:");

		//SEQUENCE
		new_state.v_seq = Transpiler::get_v_sequence(&new_state);
		if (v_deferred.size() != 0)
		{
			PLOGD << "adding deferred to v_seq...";
			for (Node* &node : v_deferred)
			{
				std::vector<Node*> v;
				v.push_back(node);
				new_state.v_seq.push_back(v);
			}
			v_deferred.clear();
		}

		for (std::vector<Node*>& v : new_state.v_seq)
			Transpiler::arrange_v(v);
		print_vec(new_state.v_seq, "sequence:");

		//REST
		new_state.v_rest = Transpiler::get_rest(&new_state);
		Transpiler::arrange_v(new_state.v_rest);
		print_vec(new_state.v_rest, "rest:");

		//handle changing branch
		if (new_state.v_seq.size() == 0 && new_state.v_rest.size() == 0)
		{
			if (check_change_block(cur_state, new_state))
			{
				Transpiler::level--;
				str_next.append(NodeToCode::indent()).append("}").append("\n");
				++pass;
				continue;
			}
			else if (check_end_for(cur_state, new_state))
			{
				Transpiler::level--;
				str_next.append(NodeToCode::indent()).append("}").append("\n");
				++pass;
				continue;
			}
			else
				break;
		}

		PLOGD << "Transpiling sequence...";
		//handle when we enter a branch
		bool should_branch_out = false;
		bool has_rest = false;
		std::vector<State> v_rest_state;
		for (std::vector<Node*>& v : new_state.v_seq)
		{
			if (!should_branch_out)
			{
				Node* node = Transpiler::transpile(v, str_next, &new_state);
				if (!node)
					continue;
				NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
				NodeFor* node_for = dynamic_cast<NodeFor*>(node);
				if (node_branch)
					should_branch_out = switch_to_branch(node_branch, cur_state, new_state);
				else if (node_for)
					should_branch_out = switch_to_for(node_for, cur_state, new_state);
			}
			else
			{
				if (v.size() == 0)
					continue;
				// PLOGD << "storing to stack the rest...";
				// State state;
				// state.v_rest = cur_state.v_rest;
				// state.is_in_else = cur_state.is_in_else;
				// v_rest_state.push_back(state);
				// has_rest = true;
			}
		}

		if (should_branch_out)
		{
			// if (has_rest)
			// {
			// 	//reorder stack
			// 	State last = Transpiler::v_states.back();
			// 	Transpiler::v_states.pop_back();
			// 	Transpiler::v_states.insert(Transpiler::v_states.end(),
			// 			v_rest_state.begin(), v_rest_state.end());
			// 	Transpiler::v_states.push_back(last);
			// }
			++pass;
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

	if (Transpiler::v_states.size() != 0)
	{
		std::string str = fmt::format("DEV - State stack should be size 0. Got {:d}", Transpiler::v_states.size());
		Transpiler::add_message(std::move(str), OUTPUT_TYPE::WARNING);
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
	Transpiler::m_temp_names.clear();
	Transpiler::v_finished_branches.clear();

	std::string str_incl = "";
	std::string str_structs = "";
	std::string str_entry = "";
	std::string str_decls = "";
	std::string str_next = "";
	std::string str_closing = "";
	std::string str_free = "";

	Transpiler::set_pre_entry(str_incl, str_structs, is_tcc);

	//entry point
	str_entry.append("int main()").append("\n");
	str_entry.append("{").append("\n");

	//find all nodes that do NOT have any LHS, this means that they are for declarations
	std::vector<Node*> v_decls;
	std::vector<Node*> v_decls_array;
	v_deferred.clear();
	get_v_decls(v_decls, v_decls_array, v_deferred);

	//begin transpiling
	Transpiler::level++;

	PLOGD << "Transpiling decls... size = " << v_decls.size();
	Transpiler::transpile_decls(v_decls, str_decls);
	PLOGD << "Transpiled decls";

	PLOGD << "Transpiling decls array... size = " << v_decls_array.size();
	Transpiler::transpile_decls_array(v_decls_array, str_decls);
	PLOGD << "Transpiled decls array";

	print_vec(v_deferred, "v_deferred:");

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
		.append(Templates::e_structs_section)

		.append(str_entry)
		.append(str_decls).append("\n")
		.append(str_next).append("\n")
		.append(str_free).append("\n")
		.append(str_closing);
	Transpiler::m_temp_names.clear();
}

Node* Transpiler::transpile(std::vector<Node*>& v, std::string& output, State* new_state)
{
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;
		bool is_it = std::find(v_iterators.begin(), v_iterators.end(), node) != v_iterators.end();
		if (is_it)
			continue;
		bool found = Transpiler::m_declared.find(node->m_name) != Transpiler::m_declared.end();
		if (found)
		{
			if (new_state && new_state->node_branch && new_state->is_in_else)
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
				bool found = std::find(Transpiler::v_finished_branches.begin(), Transpiler::v_finished_branches.end(), new_state->node_branch)
					!= Transpiler::v_finished_branches.end();
				bool is_else = false;

				if (new_state->is_in_else && !found)
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
				}
				break;
			}
		}
	}
	return nullptr;
}

void Transpiler::transpile_decls(std::vector<Node*>& v, std::string& output)
{
	//the following are only possible to be nodes without any LHS
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;
		Transpiler::m_declared.insert({node->m_name, true});
		++Transpiler::n_transpiled;

		switch (node->m_kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = dynamic_cast<NodeVariable*>(node);
				output.append(NodeToCode::comment(node));
				output.append(NodeToCode::ntc_var(node_var));
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
						NodeArray* node_array = dynamic_cast<NodeArray*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_array(node_array));
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
						NodePrint* node_print = dynamic_cast<NodePrint*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_print(node_print));
						break;
					}
					case NODE_ACTION::PROMPT:
					{
						NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(node);
						output.append(NodeToCode::comment(node));
						output.append(NodeToCode::ntc_prompt(node_prompt));
						break;
					}
				}
				break;
			}

			default:
			{
				std::string warning = fmt::format("WARNING: node {:s} is parsed as part of decls but it is not transpiled", node->m_name);
				Transpiler::add_message(std::move(warning), OUTPUT_TYPE::WARNING, node);
				break;
			}
		}
	}
}

void Transpiler::transpile_decls_array(std::vector<Node*>& v, std::string& output)
{
	//the following are only possible to be nodes without any LHS
	for (std::vector<Node*>::iterator it = v.begin();
		it != v.end();
		it++)
	{
		Node* node = *it;
		NodeArray* node_array = static_cast<NodeArray*>(node);
		bool found = Transpiler::m_declared.find(node_array->m_name) != Transpiler::m_declared.end();
		if (!found)
		{
			output.append(NodeToCode::comment(node));
			output.append(NodeToCode::ntc_array_decls(node_array));
			Transpiler::m_declared.insert({node_array->m_name, true});
			++Transpiler::n_transpiled;
		}
	}
}

std::vector<std::vector<Node*>> Transpiler::get_v_sequence(State* state)
{
	//get the independent sequence/chain of nodes
	std::vector<Node*>& v_entry = state->v_rest;
	std::vector<std::vector<Node*>> v_out;
	for (std::vector<Node*>::iterator it = v_entry.begin();
		it != v_entry.end();
		it++)
	{
		Node* node = *it;
		std::vector<Node*> v_seq = Transpiler::get_sequence(node, state);

		if (v_seq.size() == 0)
			v_seq.push_back(node);
		v_out.push_back(v_seq);
	}

	std::vector<std::vector<Node*>> v_final;
	for (std::vector<Node*>& v : v_out)
	{
		bool branch_found = false;
		std::vector<Node*> v_temp;
		for (std::vector<Node*>::iterator it = v.begin();
			it != v.end();
			it++)
		{
			Node* node = static_cast<Node*>(*it);
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
			NodeFor* node_for = dynamic_cast<NodeFor*>(node);
			if (node_branch || node_for)
			{
				branch_found = true;
				v_final.push_back({node});
			}
			else
				v_temp.push_back(node);
		}

		if (branch_found)
			v_final.push_back(v_temp);
		else
			v_final.push_back(v);
	}
	return v_final;
}

std::vector<Node*> Transpiler::get_sequence(Node* start_node, State* new_state)
{
	std::vector<Node*> v;
	std::vector<Node*> v_last;
	for (const Connection& connection : start_node->m_connections)
	{
		//in_node is the next node to check for sequence
		Node* in_node = static_cast<Node*>(connection.in_node);
		NodeBranch* node_branch = dynamic_cast<NodeBranch*>(in_node);
		NodeFor* node_for = dynamic_cast<NodeFor*>(in_node);
		if (node_branch || node_for)
		{
			//we basically want to stop a sequence once we reach a NodeBranch/NodeFor
			PLOGD << "found " << in_node->m_name << ". Stopping";
			v_last.push_back(in_node);
			continue;
		}

		if (in_node != start_node)
		{
			unsigned int count = Nodes::count_node_dep(in_node);
			if (count == 1)
			{
				if (new_state->node_branch)
				{
					//start_node is for sure a NodeBranch
					//in_node is the immediate of start_branch
					for (const Connection& connection : start_node->m_connections)
					{
						Node* nb_in_node = static_cast<Node*>(connection.in_node);
						Node* nb_out_node = static_cast<Node*>(connection.out_node);
						if (nb_in_node == in_node && nb_out_node == start_node)
						{
							const char* slot = connection.out_slot;
							bool is_true = std::strcmp(slot, "TRUE") == 0;
							bool is_false = std::strcmp(slot, "FALSE") == 0;

							if ((!new_state->is_in_else && is_true) ||
								(new_state->is_in_else && is_false))
							{
								v.push_back(in_node);
								std::vector<Node*> v2 = Transpiler::get_sequence(in_node, new_state);
								v.insert(v.end(), v2.begin(), v2.end());
							}
						}
					}
				}
				else
				{
					v.push_back(in_node);
					std::vector<Node*> v2 = Transpiler::get_sequence(in_node, new_state);
					v.insert(v.end(), v2.begin(), v2.end());
				}
			}
		}
	}
	v.insert(v.end(), v_last.begin(), v_last.end());
	return v;
}

std::vector<Node*> get_lhs_path(Node* node)
{
	std::vector<Node*> v;
	for (const Connection& connection : node->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);
		if (out_node == node)
			continue;
		v.push_back(out_node);
		std::vector<Node*> v_temp = get_lhs_path(out_node);
		v.insert(v.end(), v_temp.begin(), v_temp.end());
	}
	return v;
}

std::vector<Node*> Transpiler::get_rest(State* new_state)
{
	//get the rest to be transpiled using the last node in the sequence/chain
	std::vector<Node*> v_out;
	for (std::vector<Node*>& v : new_state->v_seq)
	{
		for (Node* &node : v)
		{
			NodeBranch* node_branch = dynamic_cast<NodeBranch*>(node);
			if (node_branch)
			{
				if (!new_state->node_branch)
					continue;
			}

			if (new_state->node_branch)
			{
				if (node_branch)
					continue;
			}

			for (const Connection& connection : node->m_connections)
			{
				Node* in_node = static_cast<Node*>(connection.in_node);
				if (in_node != node)
				{
					if (std::find(v_out.begin(), v_out.end(), in_node) != v_out.end())
						continue;
					v_out.push_back(in_node);
				}
			}
		}
	}

	//process first and reorder
	int i = 0;
	std::vector<Node*> v_final;
	for (Node* &node : v_out)
	{
		bool this_node_pushed = false;
		std::vector<Node*> v_path = get_lhs_path(node);
		for (Node* &node2 : v_path)
		{
			bool found = std::find(v_out.begin() + i, v_out.end(), node2) != v_out.end();
			if (found)
			{
				v_final.push_back(node2);
				if (!this_node_pushed)
				{
					v_final.push_back(node);
					this_node_pushed = true;
				}
			}
		}
		++i;
	}

	return v_final;
}

void Transpiler::arrange_v(std::vector<Node*>& v)
{
	std::vector<Node*> temp_v;
	std::vector<Node*>::iterator it = v.begin();
	while (it != v.end())
	{
		Node* node = *it;
		NodeArray* node_array = dynamic_cast<NodeArray*>(node);

		if (node_array)
		{
			temp_v.push_back(node_array);
			it = v.erase(it);
		}
		else
			++it;
	}
	for (Node* &node : temp_v)
		v.push_back(node);
}
}
