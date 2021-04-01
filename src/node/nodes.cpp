#include "node/nodes.hpp"

#include "core/utils.hpp"
#include "node/node_val.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"
#include "node/node_print.hpp"

namespace CodeNect
{
bool Nodes::has_built_meta = false;
std::vector<Node*> Nodes::v_nodes;
std::map<std::string, unsigned int> Nodes::m_ids
{
	{"OPERATION", 0},
	{"CAST", 0},
	{"COMPARISON", 0},
	{"BRANCH", 0},
	{"PRINT", 0},
	{"MATH", 0},
};
Nodes::m_node_t Nodes::m_available_nodes
{
	{
		"Branch", []() -> Node* {return new NodeBranch();}
	},
};

const char* Nodes::get_id(const char* id)
{
	unsigned int uid = ++Nodes::m_ids[id];
	std::string* str_id = new std::string(std::string(id) + "_" + std::to_string(uid));

	return str_id->c_str();
}

void Nodes::reset(void)
{
	for (std::pair<const std::string, unsigned int>& e : Nodes::m_ids)
	{
		e.second = 0;
	}

	Nodes::v_nodes.clear();
}

void Nodes::delete_node(std::vector<Node*>::iterator& it)
{
	delete *it;
	it = Nodes::v_nodes.erase(it);
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
				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);
				std::string str = nm->m_orig_str;
				NodePrint* node_print = new NodePrint(str, std::move(in), std::move(out));
				node_print->m_name = nm->m_name.c_str();
				node_print->m_pos = ImVec2(nm->x, nm->y);
				node_print->m_desc = nm->m_desc.c_str();
				node_print->m_override = Utils::bool_from_string(nm->m_override.c_str());
				node_print->m_append_newline = Utils::bool_from_string(nm->m_append_newline.c_str());
				Nodes::v_nodes.push_back(node_print);
				break;
			}
		}
	}

	Nodes::has_built_meta = true;
}

Node* Nodes::find_by_name(const char* name)
{
	for (Node* node : Nodes::v_nodes)
	{
		if (std::strcmp(node->m_name, name) == 0)
			return node;
	}

	PLOGE << "Can't find node by name: " << name;

	return nullptr;
}

void Nodes::build_from_meta(const std::vector<ConnectionMeta*> &v_connection_meta)
{
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
}
