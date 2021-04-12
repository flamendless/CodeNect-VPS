#ifndef _NODE_GET_HPP
#define _NODE_GET_HPP

#include "node/node.hpp"
#include "node/node_def.hpp"

namespace CodeNect
{
struct NodeGet : public Node
{
	NODE_KIND m_kind = NODE_KIND::GET;
	NODE_GET m_get = NODE_GET::EMPTY;

	inline ~NodeGet() {}
};
}

#endif //_NODE_GET_HPP
