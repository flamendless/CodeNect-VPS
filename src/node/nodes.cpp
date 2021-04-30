#include "node/nodes.hpp"

#include "ppk_assert.h"
#include "core/utils.hpp"
#include "node/node_val.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_math.hpp"
#include "node/node_ds.hpp"
#include "node/node_array.hpp"
#include "node/node_array_access.hpp"
#include "node/node_size.hpp"

namespace CodeNect
{
std::vector<Node*> Nodes::v_nodes;
Nodes::m_node_t Nodes::m_available_nodes
{
	{
		"Branch", []() -> Node* {return new NodeBranch();}
	},
};

std::map<std::string, const char*> Nodes::m_names
{
	{"POW", "POWER"},
	{"SIN", "SINE"},
	{"COS", "COSINE"},
	{"TAN", "TANGENT"},
};
std::map<const std::string, bool> Nodes::m_ids;

const char* Nodes::get_id(const char* kind)
{
	std::string orig = kind;
	std::string id = "";
	int i = 0;
	while (Nodes::m_ids.find(id) !=
		Nodes::m_ids.end())
	{
		//found
		id = orig + "_" + std::to_string(i);
		i++;
	}
	Nodes::m_ids.insert({id, true});
	std::string* ret = new std::string(id);
	return ret->c_str();
}

const char* Nodes::get_title(Node* node)
{
	switch (node->m_kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE:
		case NODE_KIND::OPERATION:
		case NODE_KIND::CAST:
		case NODE_KIND::COMPARISON:
		case NODE_KIND::BRANCH: return node->m_kind._to_string(); break;
		case NODE_KIND::ACTION:
		{
			NodeAction* node_action = static_cast<NodeAction*>(node);
			return node_action->m_action._to_string();
		}
		case NODE_KIND::MATH:
		{
			NodeMath* node_math = static_cast<NodeMath*>(node);
			switch (node_math->m_math)
			{
				case NODE_MATH::EMPTY: break;
				case NODE_MATH::ROOT: return "ROOT"; break;
				case NODE_MATH::POW:
				case NODE_MATH::SIN:
				case NODE_MATH::COS:
				case NODE_MATH::TAN: return m_names[node_math->m_math._to_string()]; break;
			}
			break;
		}
		case NODE_KIND::DS: return "DATA STRUCTURE"; break;
		case NODE_KIND::GET:
		{
			NodeGet* node_get = static_cast<NodeGet*>(node);
			return node_get->m_get._to_string();
		}
	}

	PPK_ASSERT(false, "this should not be reached");
	return "";
}

void Nodes::reset(void)
{
	for (std::pair<const std::string, bool>& e : Nodes::m_ids)
		e.second = false;

	Nodes::v_nodes.clear();
}

void Nodes::delete_node(std::vector<Node*>::iterator& it)
{
	Node* node = *it;
	Nodes::m_ids.erase(node->m_name);

	delete *it;
	it = Nodes::v_nodes.erase(it);
}

bool Nodes::check_if_no_lhs(Node* node)
{
	for (const Connection& connection : node->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);

		if (in_node == node)
			return false;
	}

	return true;
}

void Nodes::build_slots(NodeMeta& meta, v_slot_info_t& in, v_slot_info_t& out)
{
	for (std::string& input : meta.m_input_slots)
		in.push_back({input.c_str(), NODE_SLOT::_from_string(input.c_str())});

	for (std::string&  output : meta.m_output_slots)
		out.push_back({output.c_str(), NODE_SLOT::_from_string(output.c_str())});
}

void Nodes::build_from_meta(const std::vector<NodeMeta*> &v_node_meta)
{
	PLOGI << "Building nodes...";
	for (NodeMeta* nm : v_node_meta)
	{
		NODE_KIND kind = NODE_KIND::_from_string(nm->m_kind.c_str());

		switch (kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE:
			{
				NODE_SLOT value_slot = NODE_SLOT::_from_string(nm->m_value_slot.c_str());
				NodeValue val;

				switch (value_slot)
				{
					case NODE_SLOT::EMPTY: break;
					case NODE_SLOT::BOOL: val.to_bool(nm->m_value.c_str()); break;
					case NODE_SLOT::INTEGER: val.to_int(nm->m_value.c_str()); break;
					case NODE_SLOT::FLOAT: val.to_float(nm->m_value.c_str()); break;
					case NODE_SLOT::DOUBLE: val.to_double(nm->m_value.c_str()); break;
					case NODE_SLOT::STRING: val.set(nm->m_value); break;
				}

				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);

				NodeVariable* node_var = new NodeVariable(nm->m_name.c_str(), val, std::move(in), std::move(out));
				node_var->m_pos = ImVec2(nm->x, nm->y);
				node_var->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_var);
				break;
			}
			case NODE_KIND::OPERATION:
			{
				NODE_OP op = NODE_OP::_from_string(nm->m_op.c_str());
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);

				NodeOperation* node_op = new NodeOperation(op, std::move(in), std::move(out));
				node_op->m_name = nm->m_name.c_str();
				node_op->m_pos = ImVec2(nm->x, nm->y);
				node_op->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_op);
				break;
			}
			case NODE_KIND::CAST:
			{
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);

				NodeCast* node_cast = new NodeCast(std::move(in), std::move(out));
				node_cast->m_name = nm->m_name.c_str();
				node_cast->m_pos = ImVec2(nm->x, nm->y);
				node_cast->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_cast);
				break;
			}
			case NODE_KIND::COMPARISON:
			{
				NODE_CMP cmp = NODE_CMP::_from_string(nm->m_cmp.c_str());
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);

				NodeComparison* node_cmp = new NodeComparison(cmp, std::move(in), std::move(out));
				node_cmp->m_str = NodeComparison::m_cmp_str[cmp._to_string()];
				node_cmp->m_name = nm->m_name.c_str();
				node_cmp->m_pos = ImVec2(nm->x, nm->y);
				node_cmp->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_cmp);
				break;
			}
			case NODE_KIND::BRANCH:
			{
				NodeBranch* node_branch = new NodeBranch();
				node_branch->m_name = nm->m_name.c_str();
				node_branch->m_pos = ImVec2(nm->x, nm->y);
				node_branch->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_branch);
				break;
			}
			case NODE_KIND::ACTION:
			{
				NODE_ACTION action = NODE_ACTION::_from_string(nm->m_action.c_str());
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);
				std::string str = nm->m_orig_str;

				switch (action)
				{
					case NODE_ACTION::EMPTY: break;
					case NODE_ACTION::PRINT:
					{
						NodePrint* node_print = new NodePrint(str, std::move(in), std::move(out));
						node_print->m_name = nm->m_name.c_str();
						node_print->m_pos = ImVec2(nm->x, nm->y);
						node_print->m_desc = nm->m_desc.c_str();
						node_print->m_override = Utils::bool_from_string(nm->m_override.c_str());
						node_print->m_append = Utils::bool_from_string(nm->m_append.c_str());
						node_print->m_append_newline = Utils::bool_from_string(nm->m_append_newline.c_str());
						Nodes::v_nodes.push_back(node_print);
						break;
					}
					case NODE_ACTION::PROMPT:
					{
						NodePrompt* node_prompt = new NodePrompt(str, std::move(in), std::move(out));
						node_prompt->m_name = nm->m_name.c_str();
						node_prompt->m_pos = ImVec2(nm->x, nm->y);
						node_prompt->m_desc = nm->m_desc.c_str();
						node_prompt->m_override = Utils::bool_from_string(nm->m_override.c_str());
						node_prompt->m_append_newline = Utils::bool_from_string(nm->m_append_newline.c_str());
						Nodes::v_nodes.push_back(node_prompt);
						break;
					}
				}
				break;
			}
			case NODE_KIND::MATH:
			{
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);
				NODE_MATH math = NODE_MATH::_from_string(nm->m_math.c_str());
				NodeMath* node_math = new NodeMath(math, std::move(in), std::move(out));
				node_math->m_name = nm->m_name.c_str();
				node_math->m_pos = ImVec2(nm->x, nm->y);
				node_math->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_math);
				break;
			}
			case NODE_KIND::DS:
			{
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);
				NODE_DS ds = NODE_DS::_from_string(nm->m_ds.c_str());

				switch (ds)
				{
					case NODE_DS::EMPTY: break;
					case NODE_DS::ARRAY:
					{
						NODE_ARRAY array = NODE_ARRAY::_from_string(nm->m_array.c_str());
						NODE_SLOT slot = NODE_SLOT::_from_string(nm->m_array_slot.c_str());
						int size = std::stoi(nm->m_array_size);
						NodeArray* node_array = new NodeArray(array, slot, size,
							std::move(in), std::move(out));
						node_array->add_elements(nm->m_array_elements);
						node_array->m_name = nm->m_name.c_str();
						node_array->m_pos = ImVec2(nm->x, nm->y);
						node_array->m_desc = nm->m_desc.c_str();
						Nodes::v_nodes.push_back(node_array);
						break;
					}
				}
				break;
			}
			case NODE_KIND::GET:
			{
				NODE_GET get = NODE_GET::_from_string(nm->m_get.c_str());
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);
				switch (get)
				{
					case NODE_GET::EMPTY: break;
					case NODE_GET::ARRAY_ACCESS:
					{
						unsigned int index = std::stoi(nm->m_index);
						NodeArrayAccess* node_arr_access = new NodeArrayAccess(index, std::move(in), std::move(out));
						node_arr_access->m_name = nm->m_name.c_str();
						node_arr_access->m_pos = ImVec2(nm->x, nm->y);
						node_arr_access->m_desc = nm->m_desc.c_str();
						Nodes::v_nodes.push_back(node_arr_access);
						break;
					}
					case NODE_GET::SIZE:
					{
						unsigned int size = std::stoi(nm->m_size);
						NodeSize* node_size = new NodeSize(std::move(in), std::move(out));
						node_size->m_size = size;
						node_size->m_name = nm->m_name.c_str();
						node_size->m_pos = ImVec2(nm->x, nm->y);
						node_size->m_desc = nm->m_desc.c_str();
						Nodes::v_nodes.push_back(node_size);
						break;
					}
				}
			}
		}

		Nodes::reset_ids();
	}
	PLOGI << "Built nodes successfully";
}

unsigned int Nodes::count_node_dep(Node* node)
{
	unsigned int count = 0;
	for (const Connection& connection : node->m_connections)
	{
		Node* in_node = static_cast<Node*>(connection.in_node);
		if (in_node == node)
			count++;
	}
	return count;
}

Node* Nodes::find_by_name(const char* name)
{
	for (Node* &node : Nodes::v_nodes)
	{
		if (std::strcmp(node->m_name, name) == 0)
			return node;
	}
	PLOGW << "Can't find node by name: " << name;
	return nullptr;
}

Node* Nodes::find_connected_by_value(Node* node, NodeValue* target_val)
{
	for (const Connection& connection : node->m_connections)
	{
		Node* out_node = static_cast<Node*>(connection.out_node);

		if (out_node)
		{
			NodeValue* val = nullptr;
			switch (node->m_kind)
			{
				case NODE_KIND::EMPTY: break;
				case NODE_KIND::VARIABLE:
				{
					NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
					val = &node_var->m_value;
					break;
				}
				case NODE_KIND::OPERATION:
				{
					NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);
					val = node_op->m_current_val;
					break;
				}
				case NODE_KIND::CAST:
				{
					NodeCast* node_cast = dynamic_cast<NodeCast*>(out_node);
					val = node_cast->m_current_val;
					break;
				}
				case NODE_KIND::COMPARISON: break;
				case NODE_KIND::BRANCH: break;
				case NODE_KIND::ACTION: break;
				case NODE_KIND::MATH:
				{
					NodeMath* node_math = dynamic_cast<NodeMath*>(out_node);
					val = node_math->m_current_val;
					break;
				}
				case NODE_KIND::DS: break;
				case NODE_KIND::GET:
				{
					NodeGet* node_get = dynamic_cast<NodeGet*>(out_node);
					switch (node_get->m_get)
					{
						case NODE_GET::EMPTY: break;
						case NODE_GET::SIZE:
						{
							NodeSize* node_size = dynamic_cast<NodeSize*>(out_node);
							val = &node_size->m_val_size;
							break;
						}
						case NODE_GET::ARRAY_ACCESS:
						{
							NodeArrayAccess* node_arr_access = dynamic_cast<NodeArrayAccess*>(out_node);
							val = node_arr_access->m_current_val;
							break;
						}
					}
					break;
				}
			}

			if (val && val == target_val)
				return out_node;
		}
	}
	return nullptr;
}

void Nodes::build_from_meta(const std::vector<ConnectionMeta*> &v_connection_meta)
{
	PLOGI << "Building connections...";
	for (ConnectionMeta* cm : v_connection_meta)
	{
		Node* in_node = Nodes::find_by_name(cm->m_in_name.c_str());
		Node* out_node = Nodes::find_by_name(cm->m_out_name.c_str());

		if (in_node && out_node)
		{
			Connection connection;
			connection.in_node = in_node;
			connection.in_slot = cm->m_in_slot.c_str();
			connection.out_node = out_node;
			connection.out_slot = cm->m_out_slot.c_str();

			in_node->new_connection(connection);
			out_node->new_connection(connection);
		}
	}
	PLOGI << "Built connections successfully";
}

unsigned int Nodes::count_connections(void)
{
	unsigned int n = 0;

	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = *it;
		n += node->m_connections.size();
	}

	return n;
}

void Nodes::reset_ids(void)
{
	for (std::pair<const std::string, bool>& e : Nodes::m_ids)
		e.second = false;

	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = *it;
		Nodes::m_ids.insert({node->m_name, true});
	}
}
}
