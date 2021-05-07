#include "node/node_logic.hpp"

#include "node/nodes.hpp"
#include "node/node_string.hpp"
#include "node/node_var.hpp"
#include "node/node_prompt.hpp"
#include "node/node_print.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_colors.hpp"

namespace CodeNect::NodeLogic
{
void process_string(void)
{
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		NodeString* node_str = dynamic_cast<NodeString*>(*it);
		if (!node_str)
			continue;

		node_str->m_current_str.clear();
		node_str->m_from_str.clear();

		//get the LHS
		for (Connection& connection : node_str->m_connections)
		{
			Node* out_node = static_cast<Node*>(connection.out_node);
			if (out_node == node_str)
				continue;

			NodeVariable* node_var = dynamic_cast<NodeVariable*>(out_node);
			NodeString* node_str2 = dynamic_cast<NodeString*>(out_node);
			NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(out_node);
			NodeCast* node_cast = dynamic_cast<NodeCast*>(out_node);
			NodeOperation* node_op = dynamic_cast<NodeOperation*>(out_node);

			if (node_var)
				node_str->m_from_str = std::get<std::string>(node_var->m_value.data);
			else if (node_str2)
				node_str->m_from_str = node_str2->m_current_str;
			else if (node_op)
			{
				if (node_op->m_current_val)
					node_str->m_from_str = std::get<std::string>(node_op->m_current_val->data);
			}
			else if (node_prompt)
			{
				node_str->m_from_str = node_prompt->m_str;
				NodeColors::set_connection_color(connection, COLOR_TYPE::RUNTIME);
			}
			else if (node_cast)
			{
				node_str->m_from_str = std::get<std::string>(node_cast->m_current_val->data);
				NodeColors::set_connection_color(connection, COLOR_TYPE::RUNTIME);
			}
		}

		//perform string method
		switch (node_str->m_string)
		{
			case NODE_STRING::EMPTY: break;
			case NODE_STRING::TLC:
			{
				std::string tmp =  node_str->m_from_str;
				std::transform(tmp.begin(), tmp.end(), tmp.begin(),
					[](unsigned char c) { return std::tolower(c); });
				node_str->m_current_str = std::move(tmp);
				break;
			}
			case NODE_STRING::TUC:
			{
				std::string tmp =  node_str->m_from_str;
				std::transform(tmp.begin(), tmp.end(), tmp.begin(),
					[](unsigned char c) { return std::toupper(c); });
				node_str->m_current_str = std::move(tmp);
				break;
			}
			case NODE_STRING::REVERSE:
			{
				std::string tmp =  node_str->m_from_str;
				std::reverse(tmp.begin(), tmp.end());
				node_str->m_current_str = std::move(tmp);
				break;
			}
			case NODE_STRING::TOARRAY:
			{
				std::string tmp =  node_str->m_from_str;
				std::string arr = "[";
				for (int i = 0; i < tmp.length(); i++)
				{
					char c = tmp[i];
					arr.append(fmt::format("'{:c}'", c));
					if (i < tmp.length() - 1)
						arr.append(", ");
					if (i != 0 && i % 6 == 0)
						arr.append("\n");
				}
				arr.append("]");
				node_str->m_current_str = std::move(arr);
				break;
			}
		}
	}
}

//NodeArray to NodeString is not allowed
bool validate_node_string(Node* in_node, Node* out_node)
{
	NodeString* node_str = dynamic_cast<NodeString*>(in_node);
	NodeArray* node_array = dynamic_cast<NodeArray*>(out_node);
	NodePrint* node_print = dynamic_cast<NodePrint*>(out_node);

	if (node_str && (node_array || node_print))
		return false;

	return true;
}
}
