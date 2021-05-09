#include "node/node_logic.hpp"

#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"
#include "node/node_branch.hpp"
#include "node/nodes.hpp"
#include "node/node_colors.hpp"
#include "modules/debugger.hpp"

namespace CodeNect::NodeLogic
{
void process(void)
{
	Debugger::clear();

	//reset colors of connections
	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = static_cast<Node*>(*it);
		for (Connection& connection : node->m_connections)
			NodeColors::set_connection_color(connection, COLOR_TYPE::DEFAULT);
	}

	Simulation::determine_node_for_block(Nodes::v_nodes);

	// PLOGD << "start node_for check";
	// for (Node* &node : Simulation::m_tracker)
	// 	PLOGD << "\t" << node->m_name;
	// PLOGD << "end";
	// exit(1);

	for (std::vector<Node*>::iterator it = Nodes::v_nodes.begin();
		it != Nodes::v_nodes.end();
		it++)
	{
		Node* node = static_cast<Node*>(*it);
		switch (node->m_kind)
		{
			case NODE_KIND::VARIABLE:
			{
				NodeVariable* node_var = static_cast<NodeVariable*>(node);
				NodeLogic::process_var(node_var);
				break;
			}
			case NODE_KIND::MATH:
			{
				NodeMath* node_math = static_cast<NodeMath*>(node);
				NodeLogic::process_math(node_math);
				break;
			}
			case NODE_KIND::OPERATION:
			{
				NodeOperation* node_op = static_cast<NodeOperation*>(node);
				NodeLogic::process_op(node_op);
				break;
			}
			case NODE_KIND::CAST:
			{
				NodeCast* node_cast = static_cast<NodeCast*>(node);
				NodeLogic::process_cast(node_cast);
				break;
			}
			case NODE_KIND::DS:
			{
				NodeDS* node_ds = static_cast<NodeDS*>(node);
				switch (node_ds->m_ds)
				{
					case NODE_DS::EMPTY: break;
					case NODE_DS::ARRAY:
 	 	 	 	 	 {
						NodeArray* node_array = static_cast<NodeArray*>(node);
						NodeLogic::process_array(node_array);
						break;
 	 	 	 	 	 }
				}
				break;
			}
			case NODE_KIND::GET:
			{
				NodeGet* node_get = static_cast<NodeGet*>(node);
				switch (node_get->m_get)
				{
					case NODE_GET::EMPTY: break;
					case NODE_GET::ARRAY_ACCESS:
					{
						NodeArrayAccess* node_array_access = static_cast<NodeArrayAccess*>(node);
						NodeLogic::process_array_access(node_array_access);
						break;
					}
					case NODE_GET::SIZE:
					{
						NodeSize* node_size = static_cast<NodeSize*>(node);
						NodeLogic::process_size(node_size);
						break;
					}
				}
				break;
			}
			case NODE_KIND::COMPARISON:
			{
				NodeComparison* node_cmp = static_cast<NodeComparison*>(node);
				NodeLogic::process_cmp(node_cmp);
				break;
			}
			case NODE_KIND::BRANCH:
			{
				NodeBranch* node_branch = static_cast<NodeBranch*>(node);
				NodeLogic::process_branch(node_branch);
				break;
			}
			case NODE_KIND::ACTION:
			{
				NodeAction* node_action = static_cast<NodeAction*>(node);
				switch (node_action->m_action)
				{
					case NODE_ACTION::EMPTY: break;
					case NODE_ACTION::PRINT:
					{
						NodePrint* node_print = static_cast<NodePrint*>(node);
						NodeLogic::process_print(node_print);
						break;
					}
					case NODE_ACTION::PROMPT:
					{
						NodePrompt* node_prompt = static_cast<NodePrompt*>(node);
						NodeLogic::process_prompt(node_prompt);
						break;
					}
				}
				break;
			}
			case NODE_KIND::STRING:
			{
				NodeString* node_str = static_cast<NodeString*>(node);
				NodeLogic::process_string(node_str);
				break;
			}
			case NODE_KIND::LOOP:
			{
				NodeLoop* node_loop = static_cast<NodeLoop*>(node);
				switch (node_loop->m_loop)
				{
					case NODE_LOOP::EMPTY: break;
					case NODE_LOOP::FOR:
					{
						NodeFor* node_for = static_cast<NodeFor*>(node);
						NodeLogic::process_for(node_for);
						break;
					}
				}
				break;
			}
		}
	}

	NodeLogic::validate_branches();
}
}
