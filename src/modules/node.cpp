#include "modules/node.hpp"

namespace CodeNect
{
void Node::draw(void) {}

void Node::delete_connection(const Connection& connection)
{
	for (v_connection_t::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
	{
		if (connection == *it)
		{
			m_connections.erase(it);
			break;
		}
	}
}
}
