#include "node/node_logic.hpp"

namespace CodeNect::NodeLogic
{
void process(void)
{
	NodeLogic::process_var();
	NodeLogic::process_math();
	NodeLogic::process_op();
	NodeLogic::process_cast();
	NodeLogic::process_cmp();
	NodeLogic::process_branch();
	NodeLogic::process_print();
	NodeLogic::process_prompt();
}
}
