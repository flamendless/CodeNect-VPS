#ifndef _SIMULATION_HPP
#define _SIMULATION_HPP

#include <vector>
#include "node/node.hpp"
#include "node/node_for.hpp"

namespace CodeNect
{
struct ForState
{
	NodeFor* m_node_for = nullptr;
	std::vector<Node*> m_nodes;
};

struct Simulation
{
	static std::vector<ForState> m_v_stack;
	static std::vector<Node*> m_v_tracker;

	Simulation() = delete;
	static void iterate(int dir);
	static void reset(void);

	static bool is_in_for(Node*);
	static void simulate(Node*);
	static std::vector<Node*> get_node_for_block(Node*);
	static void determine_node_for_block(std::vector<Node*>&);
};
}

#endif //_SIMULATION_HPP
