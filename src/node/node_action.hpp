#ifndef _NODE_ACTION_HPP
#define _NODE_ACTION_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeAction : public Node
{
	NODE_KIND m_kind = NODE_KIND::ACTION;
	NODE_ACTION m_action = NODE_ACTION::EMPTY;

	inline ~NodeAction() {}
};
}

#endif //_NODE_ACTION_HPP
