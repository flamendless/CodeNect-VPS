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

	bool operator==(const Connection& other) const;
	bool operator!=(const Connection& other) const;
};
}

#endif //_CONNECTION_HPP
