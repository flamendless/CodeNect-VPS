#ifndef _CONNECTION_HPP
#define _CONNECTION_HPP

#include <string>

namespace CodeNect
{
enum class COLOR_TYPE { DEFAULT = 0, SUCCESS, FAIL, RUNTIME};

struct Connection
{
	void* in_node;
	const char* in_slot;
	const char* in_slot_mod;

	void* out_node;
	const char* out_slot;
	const char* out_slot_mod;

	//this determines the data flow
	//used in node_branch
	COLOR_TYPE color = COLOR_TYPE::DEFAULT;
	bool is_valid = true;

	bool operator==(const Connection& other) const;
	bool operator!=(const Connection& other) const;
};
}

#endif //_CONNECTION_HPP
