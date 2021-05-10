#include "modules/simulation.hpp"

#include <algorithm>
#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/node.hpp"

namespace CodeNect
{
std::vector<ForState> Simulation::m_v_stack;
std::vector<Node*> Simulation::m_v_tracker;

void Simulation::iterate(int dir)
{
	for (ForState& state : Simulation::m_v_stack)
	{
		NodeFor* node_for = state.m_node_for;
		node_for->m_override_it += dir;
	}
}

void Simulation::reset(void)
{
	for (ForState& state : Simulation::m_v_stack)
	{
		NodeFor* node_for = state.m_node_for;
		node_for->m_override_it = node_for->m_cur_start_index;
	}
}

bool Simulation::is_in_for(Node* node)
{
	return std::find(Simulation::m_v_tracker.begin(), Simulation::m_v_tracker.end(), node)
		!= Simulation::m_v_tracker.end();
}

void Simulation::simulate(Node* node)
{
}

std::vector<Node*> Simulation::get_node_for_block(Node* node)
{
	std::vector<Node*> v;
	for (const Connection& connection : node->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		if (in_node == node)
			continue;
		v.push_back(in_node);
		std::vector<Node*> v2 = Simulation::get_node_for_block(in_node);
		for (Node* &node : v2)
		{
			bool found = std::find(v.begin(), v.end(), node) != v.end();
			if (!found)
			{
				v2.push_back(node);
				Simulation::m_v_tracker.push_back(node);
			}
		}
	}
	return v;
}

void Simulation::determine_node_for_block(std::vector<Node*>& v_nodes)
{
	Simulation::m_v_stack.clear();
	Simulation::m_v_tracker.clear();

	for (std::vector<Node*>::iterator it = v_nodes.begin();
		it != v_nodes.end();
		it++)
	{
		Node* node = static_cast<Node*>(*it);
		NodeFor* node_for = dynamic_cast<NodeFor*>(node);
		if (!node_for)
			continue;
		ForState state;
		state.m_node_for = node_for;
		state.m_nodes = Simulation::get_node_for_block(node_for);
		Simulation::m_v_stack.push_back(std::move(state));
	}

	// PLOGD << "start node_for check";
	// for (ForState& state : Simulation::m_v_stack)
	// {
	// 	PLOGD << state.m_node_for->m_name;
	// 	for (Node* &node : state.m_nodes)
	// 		PLOGD << "\t" << node->m_name;
	// }
	// PLOGD << "end";
	// exit(1);
}
}
