#include "node/node_logic.hpp"
#include "node/node_entry.hpp"
#include "node/node_var.hpp"
#include "node/node_array.hpp"
#include "node/node_print.hpp"
#include "node/node_prompt.hpp"

namespace CodeNect::NodeLogic
{
void process(void)
{
	NodeLogic::process_var();
	NodeLogic::process_math();
	NodeLogic::process_op();
	NodeLogic::process_cast();
	NodeLogic::process_array();
	NodeLogic::process_array_access();
	NodeLogic::process_size();
	NodeLogic::process_cmp();
	NodeLogic::process_branch();
	NodeLogic::process_print();
	NodeLogic::process_prompt();
}

//only the following are allowed to be connected to NodeEntry:
//* NodeVariable
//* NodeArray
//* NodePrint
//* NodePrompt
bool validate_node_entry(Node* in_node, Node* out_node)
{
	NodeEntry* node_entry = dynamic_cast<NodeEntry*>(out_node);
	NodeVariable* node_var = dynamic_cast<NodeVariable*>(in_node);
	NodeArray* node_array = dynamic_cast<NodeArray*>(in_node);
	NodePrint* node_print = dynamic_cast<NodePrint*>(in_node);
	NodePrompt* node_prompt = dynamic_cast<NodePrompt*>(in_node);

	if (node_entry)
	{
		if (node_var || node_array || node_print || node_prompt)
			return true;
		return false;
	}

	return true;
}
}
