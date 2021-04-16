#include "node/node_logic.hpp"
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
}
