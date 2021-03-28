#ifndef _CONNECTION_HPP
#define _CONNECTION_HPP

namespace CodeNect
{
enum COLOR_TYPE { DEFAULT = 0, TRUE, FALSE };

struct Connection
{
	void* in_node;
	const char* in_slot;

	void* out_node;
	const char* out_slot;

	//this determines the data flow
	//used in node_branch
	COLOR_TYPE color = COLOR_TYPE::DEFAULT;
	bool is_valid = true;

	bool operator==(const Connection& other) const;
	bool operator!=(const Connection& other) const;
};
}

#endif //_CONNECTION_HPP
