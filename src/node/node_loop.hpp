#ifndef _NODE_LOOP_HPP
#define _NODE_LOOP_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeLoop : public Node
{
	NODE_KIND m_kind = NODE_KIND::LOOP;
	NODE_LOOP m_loop = NODE_LOOP::EMPTY;

	inline ~NodeLoop() {}
};
}

#endif //_NODE_LOOP_HPP
