#include "modules/simulation.hpp"

#include <algorithm>
#include "ppk_assert.h"
#include "plog/Log.h"
#include "node/node.hpp"

namespace CodeNect
{
std::vector<ForState> Simulation::m_v_stack;
std::vector<Node*> Simulation::m_v_tracker;
bool Simulation::is_playing = false;
Timer* Simulation::timer = nullptr;

void Simulation::iterate(int dir)
{
	for (ForState& state : Simulation::m_v_stack)
	{
		bool should_iterate = true;
		for (Node* &node : state.m_nodes)
		{
			NodeFor* node_for = dynamic_cast<NodeFor*>(node);
			if (!node_for)
				continue;
			for (ForState& state : Simulation::m_v_stack)
			{
				if (state.m_node_for == node_for)
				{
					if (!state.m_node_for->m_has_reached_end)
						should_iterate = false;
					else
						state.m_node_for->m_has_reached_end = false;
				}
			}
		}

		if (!should_iterate)
			continue;

		NodeFor* node_for = state.m_node_for;
		int temp = node_for->m_override_it + (dir * node_for->m_cur_increment);
		bool is_over = false;
		bool is_under = false;
		int last;

		switch (node_for->m_cmp)
		{
			case NODE_CMP::EMPTY: break;
			case NODE_CMP::AND: break;
			case NODE_CMP::OR: break;
			case NODE_CMP::EQ: break;
			case NODE_CMP::NEQ: break;
			case NODE_CMP::LT:
			{
				is_over = !(temp < node_for->m_cur_end_index);
				is_under = !(temp >= node_for->m_cur_start_index);
				last = node_for->m_cur_end_index - 1;
				break;
			}
			case NODE_CMP::GT:
			{
				is_over = !(temp > node_for->m_cur_end_index);
				is_under = !(temp <= node_for->m_cur_start_index);
				last = node_for->m_cur_start_index + 1;
				break;
			}
			case NODE_CMP::LTE:
			{
				is_over = !(temp <= node_for->m_cur_end_index);
				is_under = !(temp > node_for->m_cur_start_index);
				last = node_for->m_cur_end_index;
				break;
			}
			case NODE_CMP::GTE:
			{
				is_over = !(temp >= node_for->m_cur_end_index);
				is_under = !(temp < node_for->m_cur_start_index);
				last = node_for->m_cur_start_index;
				break;
			}
		}

		if (is_over)
		{
			node_for->m_override_it = node_for->m_cur_start_index;
			node_for->m_has_reached_end = true;
		}
		else if (is_under)
			node_for->m_override_it = last;
		else
			node_for->m_override_it = temp;
	}
}

void Simulation::reset(void)
{
	Simulation::is_playing = false;
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

void Simulation::update(float dt)
{
	if (!Simulation::is_playing)
		return;

	Simulation::timer->m_current_sec += dt;
	if (Simulation::timer->m_current_sec > Simulation::timer->m_max_sec)
	{
		Simulation::iterate(1);
		Simulation::timer->m_current_sec = 0;
		if (!Simulation::timer->m_repeat)
		{
			Simulation::is_playing = false;
			return;
		}
	}
}

void Simulation::play(float max_timer, bool repeat)
{
	if (Simulation::is_playing)
	{
		Simulation::is_playing = false;
		return;
	}

	if (Simulation::timer)
	{
		delete Simulation::timer;
		Simulation::timer = nullptr;
	}

	Simulation::timer = new Timer();
	timer->m_current_sec = 0.0f;
	timer->m_max_sec = max_timer;
	timer->m_repeat = repeat;
	Simulation::is_playing = true;
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
