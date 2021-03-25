#include "node/connection.hpp"

namespace CodeNect
{
bool Connection::operator==(const Connection& other) const
{
	return in_node == other.in_node &&
		in_slot == other.in_slot &&
		out_node == other.out_node &&
		out_slot == other.out_slot;
}

bool Connection::operator!=(const Connection& other) const
{
	return !operator ==(other);
}
}
