#ifndef _CONNECTION_HPP
#define _CONNECTION_HPP

namespace CodeNect
{
struct Connection
{
	void* in_node;
	const char* in_slot;

	void* out_node;
	const char* out_slot;

	inline bool operator==(const Connection& other) const
	{
		return in_node == other.in_node &&
			in_slot == other.in_slot &&
			out_node == other.out_node &&
			out_slot == other.out_slot;
	}

	inline bool operator!=(const Connection& other) const
	{
		return !operator ==(other);
	}
};
}

#endif //_CONNECTION_HPP