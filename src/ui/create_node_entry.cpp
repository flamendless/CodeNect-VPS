#include "ui/create_node.hpp"

#include "node/nodes.hpp"
#include "node/node_entry.hpp"

namespace CodeNect
{
void CreateNode::create_node_entry(void)
{
	TempEntryData* tmp = std::get<TempEntryData*>(data);

	v_slot_info_t&& out = {};
	out.push_back({tmp->slot_out._to_string(), tmp->slot_out});
	NodeEntry* node = new NodeEntry(std::move(out));
	Nodes::v_nodes.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());
	Nodes::has_entry = true;
}

void CreateNode::draw_entry(void)
{
	if (CreateNode::is_edit_mode)
		return;

	TempEntryData* tmp = std::get<TempEntryData*>(data);

	if (ImGui::BeginCombo("Output Type", tmp->slot_out._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, tmp->slot_out._to_string() == txt))
				tmp->slot_out = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	tmp->valid_entry = tmp->slot_out != +NODE_SLOT::EMPTY;
	CreateNode::can_create = tmp->valid_entry;
}
}
