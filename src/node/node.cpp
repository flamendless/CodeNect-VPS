#include "node/node.hpp"

namespace CodeNect
{
void Node::set_desc(char* desc)
{
	std::string* str = new std::string(desc);
	m_desc = str->c_str();
}

void Node::new_connection(const Connection& new_connection)
{
	m_connections.push_back(new_connection);
}

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
