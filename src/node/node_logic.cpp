#include "node/node_logic.hpp"
#include "node/nodes.hpp"

namespace CodeNect::NodeLogic
{
void process(void)
{
	NodeLogic::process_var();
	NodeLogic::process_op();
	NodeLogic::process_cast();
	NodeLogic::process_cmp();
	NodeLogic::process_branch();
}
}
