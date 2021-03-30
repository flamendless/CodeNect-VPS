#include "ui/create_node.hpp"

#include "core/utils.hpp"
#include "node/nodes.hpp"
#include "node/node_var.hpp"
#include "node/node_op.hpp"
#include "node/node_cast.hpp"
#include "node/node_cmp.hpp"
#include "node/node_branch.hpp"
#include "node/node_print.hpp"

namespace CodeNect
{
ImGuiWindowFlags CreateNode::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
	ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse |
	ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
bool CreateNode::is_first = false;
bool CreateNode::is_open = false;
bool CreateNode::is_pos_locked = true;
bool CreateNode::is_edit_mode = false;
const char* CreateNode::title = ICON_FA_PLUS_SQUARE "Create Node";
NODE_KIND CreateNode::kind = NODE_KIND::EMPTY;
Node* CreateNode::node_to_edit;
bool CreateNode::can_create = false;
std::variant<
		TempVarData*, TempOperationData*,
		TempCastData*, TempComparisonData*,
		TempBranchData*, TempPrintData*
	>CreateNode::data;

void CreateNode::open(NODE_KIND kind)
{
	switch (kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE:
		{
			CreateNode::data = new TempVarData();
			break;
		}
		case NODE_KIND::OPERATION:
		{
			CreateNode::data = new TempOperationData();
			break;
		}
		case NODE_KIND::CAST:
		{
			CreateNode::data = new TempCastData();
			break;
		}
		case NODE_KIND::COMPARISON:
		{
			CreateNode::data = new TempComparisonData();
			break;
		}
		case NODE_KIND::BRANCH:
		{
			CreateNode::data = new TempBranchData();
			break;
		}
		case NODE_KIND::PRINT:
		{
			CreateNode::data = new TempPrintData();
			break;
		}
	}

	CreateNode::kind = kind;
	CreateNode::is_open = true;
	CreateNode::is_first = true;
}

void CreateNode::edit(Node* node)
{
	switch (node->m_kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE:
		{
			NodeVariable* node_var = static_cast<NodeVariable*>(node);
			NodeValue* val = &node_var->m_value_orig;
			NODE_SLOT slot = NODE_SLOT::_from_string(node_var->m_out_slots[0].title);
			TempVarData* temp = new TempVarData();

			std::strcpy(temp->buf_name, node_var->m_name);
			std::strcpy(temp->buf_desc, node_var->m_desc);
			temp->slot = slot;
			temp->value.copy(*val);
			temp->valid_value = true;
			temp->valid_name = true;

			switch (slot)
			{
				case NODE_SLOT::EMPTY: break;
				case NODE_SLOT::BOOL: break;
				case NODE_SLOT::INTEGER: temp->temp_int = std::get<int>(val->data); break;
				case NODE_SLOT::FLOAT: temp->temp_float = std::get<float>(val->data); break;
				case NODE_SLOT::DOUBLE: temp->temp_double = std::get<double>(val->data); break;
				case NODE_SLOT::STRING: std::strcpy(temp->buf_string, std::get<std::string>(val->data).c_str()); break;
			}

			CreateNode::node_to_edit = node;
			CreateNode::data = temp;
			break;
		}
		case NODE_KIND::OPERATION:
		{
			NodeOperation* node_op = static_cast<NodeOperation*>(node);
			TempOperationData* temp = new TempOperationData();
			NODE_SLOT slot = NODE_SLOT::_from_string(node_op->m_out_slots[0].title);

			std::strcpy(temp->buf_desc, node_op->m_desc);
			temp->slot = slot;
			temp->op = node_op->m_op;
			temp->valid_op = true;

			CreateNode::node_to_edit = node;
			CreateNode::data = temp;
			break;
		}
		case NODE_KIND::CAST:
		{
			NodeCast* node_cast = static_cast<NodeCast*>(node);
			TempCastData* temp = new TempCastData();
			NODE_SLOT slot_in = NODE_SLOT::_from_string(node_cast->m_in_slots[0].title);
			NODE_SLOT slot_out = NODE_SLOT::_from_string(node_cast->m_out_slots[0].title);

			std::strcpy(temp->buf_desc, node_cast->m_desc);
			temp->slot_in = slot_in;
			temp->slot_out = slot_out;
			temp->valid_cast = true;

			CreateNode::node_to_edit = node;
			CreateNode::data = temp;
			break;
		}
		case NODE_KIND::COMPARISON:
		{
			NodeComparison* node_cmp = static_cast<NodeComparison*>(node);
			TempComparisonData* temp = new TempComparisonData();
			NODE_SLOT slot_in = NODE_SLOT::_from_string(node_cmp->m_in_slots[0].title);
			NODE_CMP cmp = node_cmp->m_cmp;

			std::strcpy(temp->buf_desc, node_cmp->m_desc);
			temp->slot_in = slot_in;
			temp->str = NodeComparison::m_cmp_str[cmp._to_string()];
			temp->cmp = cmp;
			temp->valid_cmp = true;

			CreateNode::node_to_edit = node;
			CreateNode::data = temp;
			break;
		}
		case NODE_KIND::BRANCH:
		{
			NodeBranch* node_branch = static_cast<NodeBranch*>(node);
			TempBranchData* temp = new TempBranchData();

			std::strcpy(temp->buf_desc, node_branch->m_desc);
			temp->valid_branch = true;

			CreateNode::node_to_edit = node;
			CreateNode::data = temp;
			break;
		}
		case NODE_KIND::PRINT:
		{
			NodePrint* node_print = static_cast<NodePrint*>(node);
			TempPrintData* temp = new TempPrintData();

			std::strcpy(temp->buf_desc, node_print->m_desc);
			temp->valid_print = true;

			CreateNode::node_to_edit = node;
			CreateNode::data = temp;
			break;
		}
	}

	CreateNode::kind = node->m_kind;
	CreateNode::is_open = true;
	CreateNode::is_edit_mode = true;
	CreateNode::is_first = true;
}

void CreateNode::close(void)
{
	CreateNode::kind = NODE_KIND::EMPTY;
	CreateNode::is_open = false;

	if (CreateNode::is_edit_mode)
	{
		CreateNode::is_edit_mode = false;
		CreateNode::node_to_edit = nullptr;
	}
}

void CreateNode::draw(void)
{
	if (!CreateNode::is_open)
		return;

	if (CreateNode::is_first)
	{
		ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		CreateNode::is_first = false;
	}

	if (ImGui::Begin(CreateNode::title, &CreateNode::is_open, CreateNode::flags))
	{
		Utils::center_text(CreateNode::title, true);
		ImGui::Separator();

		switch (CreateNode::kind)
		{
			case NODE_KIND::EMPTY: break;
			case NODE_KIND::VARIABLE: CreateNode::draw_var(); break;
			case NODE_KIND::OPERATION: CreateNode::draw_op(); break;
			case NODE_KIND::CAST: CreateNode::draw_cast(); break;
			case NODE_KIND::COMPARISON: CreateNode::draw_cmp(); break;
			case NODE_KIND::BRANCH: break;
			case NODE_KIND::PRINT: CreateNode::draw_print(); break;
		}

		CreateNode::draw_desc();
		ImGui::Separator();
		CreateNode::draw_buttons();

		ImGui::End();
	}
}

void CreateNode::draw_desc(void)
{
	char* buf = nullptr;

	switch (CreateNode::kind)
	{
		case NODE_KIND::EMPTY: break;
		case NODE_KIND::VARIABLE: buf = std::get<TempVarData*>(data)->buf_desc; break;
		case NODE_KIND::OPERATION: buf = std::get<TempOperationData*>(data)->buf_desc; break;
		case NODE_KIND::CAST: buf = std::get<TempCastData*>(data)->buf_desc; break;
		case NODE_KIND::COMPARISON: buf = std::get<TempComparisonData*>(data)->buf_desc; break;
		case NODE_KIND::BRANCH: buf = std::get<TempBranchData*>(data)->buf_desc; break;
		case NODE_KIND::PRINT: buf = std::get<TempPrintData*>(data)->buf_desc; break;
	}

	ImGui::InputText("Description", buf, BUF_SIZE * 2);
	Utils::help_marker("This will serve as comments", true);
}

void CreateNode::draw_buttons(void)
{
	if (!CreateNode::can_create)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

	const char* str = ICON_FA_CHECK " Create";

	if (CreateNode::is_edit_mode)
		str = ICON_FA_CHECK " Edit";

	if (ImGui::Button(str))
	{
		if (CreateNode::can_create)
		{
			switch (CreateNode::kind)
			{
				case NODE_KIND::EMPTY: break;
				case NODE_KIND::VARIABLE: CreateNode::create_node_var(); break;
				case NODE_KIND::OPERATION: CreateNode::create_node_op(); break;
				case NODE_KIND::CAST: CreateNode::create_node_cast(); break;
				case NODE_KIND::COMPARISON: CreateNode::create_node_cmp(); break;
				case NODE_KIND::BRANCH: CreateNode::create_node_branch(); break;
				case NODE_KIND::PRINT: CreateNode::create_node_print(); break;
			}

			CreateNode::close();
		}
	}

	if (!CreateNode::can_create)
		ImGui::PopStyleVar(1);

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_TIMES " Cancel"))
		CreateNode::close();

	ImGui::SameLine();

	const char* icon = CreateNode::is_pos_locked ? ICON_FA_LOCK : ICON_FA_LOCK_OPEN;

	if (ImGui::Checkbox(icon, &CreateNode::is_pos_locked))
	{
		if (CreateNode::is_pos_locked)
			CreateNode::flags |= ImGuiWindowFlags_NoMove;
		else
			CreateNode::flags &= ~ImGuiWindowFlags_NoMove;
	}

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Lock/Unlock the position of this window");
}
}
