#ifndef _NODE_BRANCH_HPP
#define _NODE_BRANCH_HPP

#include "node/node_def.hpp"
#include "node/node.hpp"

namespace CodeNect
{
struct NodeBranch : public Node
{
	NODE_KIND m_kind = NODE_KIND::BRANCH;

	explicit NodeBranch();
	inline ~NodeBranch() {}
};
}

#endif //_NODE_BRANCH_HPP
