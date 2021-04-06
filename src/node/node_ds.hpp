#ifndef _NODE_DS_HPP
#define _NODE_DS_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeDS : public Node
{
	NODE_KIND m_kind = NODE_KIND::DS;
	NODE_DS m_ds = NODE_DS::EMPTY;

	inline ~NodeDS() {}
};
}

#endif //_NODE_DS_HPP
