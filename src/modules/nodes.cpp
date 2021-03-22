#include "modules/nodes.hpp"
#include "modules/node_val.hpp"
#include "modules/node_var.hpp"
#include "modules/node_op.hpp"

namespace CodeNect
{
bool Nodes::has_built_meta = false;
std::vector<Node*> Nodes::v_nodes;
std::vector<NodeVariable*> Nodes::v_nodes_var;
std::vector<NodeOperation*> Nodes::v_nodes_op;

Nodes::m_node_t Nodes::m_available_nodes
{
	{
		"Test_Int", []() -> Node*
		{
			NodeValue value;
			value.set(1);

			return new NodeVariable("a", value,
				{{"INTEGER", NODE_SLOT::INTEGER}},
				{{"INTEGER", NODE_SLOT::INTEGER}}
			);
		}
	},
	{
		"Test_Int2", []() -> Node*
		{
			NodeValue value;
			value.set(2);

			return new NodeVariable("b", value,
				{{"INTEGER", NODE_SLOT::INTEGER}},
				{{"INTEGER", NODE_SLOT::INTEGER}}
			);
		}
	},
	{
		"Test_Res_Int", []() -> Node*
		{
			NodeValue value;
			value.set(0);

			return new NodeVariable("res", value,
				{{"INTEGER", NODE_SLOT::INTEGER}},
				{{"INTEGER", NODE_SLOT::INTEGER}}
			);
		}
	},
	{
		"Test_Add_Int", []() -> Node*
		{
			return new NodeOperation(NODE_OP::ADD,
				{{"INTEGER", NODE_SLOT::INTEGER}},
				{{"INTEGER", NODE_SLOT::INTEGER}}
			);
		}
	}
};

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
					case NODE_SLOT::STRING: val.set(nm->m_value.c_str()); break;
				}

				v_slot_info_t&& in = {};
				v_slot_info_t&& out = {};
				Nodes::build_slots(*nm, in, out);

				NodeVariable* node_var = new NodeVariable(nm->m_name.c_str(), val, std::move(in), std::move(out));
				node_var->m_pos = ImVec2(nm->x, nm->y);
				node_var->m_desc = nm->m_desc.c_str();
				Nodes::v_nodes.push_back(node_var);
				Nodes::v_nodes_var.push_back(node_var);

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
				Nodes::v_nodes_op.push_back(node_op);

				break;
			}
			case NODE_KIND::IF: break;
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

			in_node->m_connections.push_back(connection);
			out_node->m_connections.push_back(connection);
		}
	}
}
}
