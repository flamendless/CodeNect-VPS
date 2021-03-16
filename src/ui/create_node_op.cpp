#include "ui/create_node.hpp"

#include "modules/node.hpp"
#include "modules/nodes.hpp"
#include "modules/node_op.hpp"

namespace CodeNect
{
void CreateNode::create_op_node(void)
{
	v_slot_info_t&& in = {};
	v_slot_info_t&& out = {};

	for (const NODE_SLOT& slot : data::op->v_slots_in)
	{
		in.push_back({slot._to_string(), slot});
	}

	for (const NODE_SLOT& slot : data::op->v_slots_out)
	{
		out.push_back({slot._to_string(), slot});
	}

	NodeOp* node = new NodeOp(data::op->op._to_string(), std::move(in), std::move(out));

	Nodes::v_nodes.push_back(node);
	ImNodes::AutoPositionNode(Nodes::v_nodes.back());
}

void CreateNode::draw_op(void)
{
	if (ImGui::BeginCombo("Operation Method", data::op->op._to_string()))
	{
		for (NODE_OP op : NODE_OP::_values())
		{
			if (op == +NODE_OP::EMPTY)
				continue;

			ImGui::PushID(op);
			const char* txt = op._to_string();

			if (ImGui::Selectable(txt, data::op->op._to_string() == txt))
			{
				data::op->op = op;
				data::op->valid_op = true;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	if (!data::op->valid_op)
		return;

	CreateNode::draw_opt_input();
	CreateNode::draw_slots();
	CreateNode::can_create = (data::op->v_slots_in.size() >= 2) &&
		(data::op->v_slots_out.size() != 0);
}

void draw_slots_v(const char* str, std::vector<NODE_SLOT> &v)
{
	if (v.size() > 0)
	{
		ImGui::BeginGroup();
		ImGui::Text("%s", str);
		ImGui::Indent();

		for (std::vector<NODE_SLOT>::iterator it = v.begin(); it != v.end();)
		{
			NODE_SLOT slot = *it;
			ImGui::PushID(slot);
			bool pressed = false;

			if (ImGui::SmallButton(ICON_FA_MINUS))
			{
				it = v.erase(it);
				pressed = true;
			}

			ImGui::PopID();
			ImGui::SameLine();
			ImGui::BulletText("%s", slot._to_string());

			if (!pressed)
				it++;
		}

		ImGui::Unindent();
		ImGui::EndGroup();
	}
}

void CreateNode::draw_slots(void)
{
	std::vector<NODE_SLOT>* in = &data::op->v_slots_in;
	std::vector<NODE_SLOT>* out = &data::op->v_slots_out;
	const int in_size = in->size();
	const int out_size = out->size();

	if (in_size > 0 || out_size > 0)
		ImGui::Separator();

	const float w = ImGui::GetContentRegionAvailWidth();
	ImGui::SetNextItemWidth(w/2);
	draw_slots_v("Input Slots", *in);
	ImGui::SameLine(w/2);
	draw_slots_v("Output Slots", *out);
}

void CreateNode::draw_opt_input(void)
{
	static NODE_SLOT temp_slot = NODE_SLOT::EMPTY;

	if (ImGui::BeginCombo("Type", temp_slot._to_string()))
	{
		for (NODE_SLOT slot : NODE_SLOT::_values())
		{
			if (slot == +NODE_SLOT::EMPTY || slot == +NODE_SLOT::BOOL)
				continue;

			ImGui::PushID(slot);
			const char* txt = slot._to_string();

			if (ImGui::Selectable(txt, temp_slot._to_string() == txt))
				temp_slot = slot;

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	if (temp_slot != +NODE_SLOT::EMPTY)
	{
		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_PLUS " in"))
		{
			data::op->v_slots_in.push_back(temp_slot);
			temp_slot = NODE_SLOT::EMPTY;
		}

		ImGui::SameLine();

		if (ImGui::SmallButton(ICON_FA_PLUS " out"))
		{
			data::op->v_slots_out.push_back(temp_slot);
			temp_slot = NODE_SLOT::EMPTY;
		}
	}
}
}
