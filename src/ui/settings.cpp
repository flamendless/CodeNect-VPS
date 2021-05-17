#include "settings.hpp"

#include <string>
#include "imgui_internal.h"
#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "core/config.hpp"
#include "core/utils.hpp"
#include "ui/alert.hpp"
#include "core/commands.hpp"
#include "ui/command_palette.hpp"
#include "ui/node_interface.hpp"
#include "core/font.hpp"
#include "core/settings_data.hpp"

namespace CodeNect
{
bool was_changed = false;
StyleData style_data;
CommandPaletteData cp_data;
SidebarData sb_data;
NodeInterfaceData ni_data;

ImGuiWindowFlags Settings::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize;
bool Settings::is_open = false;
bool Settings::is_first = false;
bool Settings::is_pos_locked = true;
const char* Settings::title = ICON_FA_COG " SETTINGS";

void Settings::init(void)
{
	was_changed = false;
	style_data.changed = false;
	cp_data.changed = false;
	sb_data.changed = false;
	ni_data.changed = false;

	style_data.style = STYLE::_from_string(Config::style.c_str());
	style_data.font_size = Config::font_size;
	style_data.font = Config::font;
	cp_data.size = Config::CommandPalette_c::size;
	sb_data.fade_in = Config::Sidebar_c::fade_in;
	sb_data.fade_out = Config::Sidebar_c::fade_out;

	ni_data.title_color[0] = Config::NodeInterface_c::title_color.x;
	ni_data.title_color[1] = Config::NodeInterface_c::title_color.y;
	ni_data.title_color[2] = Config::NodeInterface_c::title_color.z;
	ni_data.label_color[0] = Config::NodeInterface_c::label_color.x;
	ni_data.label_color[1] = Config::NodeInterface_c::label_color.y;
	ni_data.label_color[2] = Config::NodeInterface_c::label_color.z;
	ni_data.item_inner_spacing[0] = Config::NodeInterface_c::item_inner_spacing.x;
	ni_data.item_inner_spacing[1] = Config::NodeInterface_c::item_inner_spacing.y;

	ni_data.title_color_orig[0] = Config::NodeInterface_c::title_color.x;
	ni_data.title_color_orig[1] = Config::NodeInterface_c::title_color.y;
	ni_data.title_color_orig[2] = Config::NodeInterface_c::title_color.z;
	ni_data.label_color_orig[0] = Config::NodeInterface_c::label_color.x;
	ni_data.label_color_orig[1] = Config::NodeInterface_c::label_color.y;
	ni_data.label_color_orig[2] = Config::NodeInterface_c::label_color.z;
	ni_data.item_inner_spacing_orig[0] = Config::NodeInterface_c::item_inner_spacing.x;
	ni_data.item_inner_spacing_orig[1] = Config::NodeInterface_c::item_inner_spacing.y;
}

void Settings::register_commands(void)
{
	Command* cmd = new Command("Settings", "open settings window", ICON_FA_COG);
	cmd->set_fn(Settings::open);
	cmd->m_close_command_palette = true;

	Commands::register_cmd(*cmd);
}

void Settings::open(void)
{
	Settings::init();
	Settings::is_first = true;
	Settings::is_open = true;
	PLOGD << "Opened Settings window";
}

void Settings::close(void)
{
	if (was_changed)
		Settings::reset_values();

	style_data.changed = false;
	cp_data.changed = false;
	sb_data.changed = false;
	ni_data.changed = false;
	was_changed = false;
	Settings::is_open = false;
	Settings::is_first = true;
}

void Settings::draw(void)
{
	if (!Settings::is_open)
		return;

	if (Settings::is_first)
	{
		ImVec2 center_pos(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		Settings::is_first = false;
	}

	if (ImGui::Begin("Settings", &Settings::is_open, Settings::flags))
	{
		Utils::center_text(Settings::title, true);
		ImGui::Separator();

		Utils::center_text(ICON_FA_HOME " General", true);
		Settings::draw_theme_select();
		Settings::draw_font_select();
		ImGui::Separator();
		Settings::draw_sidebar();
		ImGui::Separator();
		Settings::draw_command_palette();
		ImGui::Separator();
		Settings::draw_node_interface();

		was_changed = style_data.changed || cp_data.changed ||
			sb_data.changed || ni_data.changed;

		if (was_changed)
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "* Save and restart needed");

		ImGui::Separator();

		Settings::draw_buttons();

		ImGui::End();
	}

	if (ImGui::IsKeyPressedMap(ImGuiKey_Escape))
		Settings::is_open = false;
}

void Settings::draw_theme_select(void)
{
	if (ImGui::BeginCombo(ICON_FA_PALETTE " Theme", style_data.style._to_string()))
	{
		for (STYLE style : STYLE::_values())
		{
			const char* txt = style._to_string();

			if (style == +STYLE::EMPTY)
				continue;

			ImGui::PushID(style);

			if (ImGui::Selectable(txt, style_data.style._to_string() == txt))
			{
				style_data.style = style;
				NodeInterface::has_changed_theme = true;
				style_data.changed = true;
				Config::set_style(style._to_index());
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}

void Settings::draw_font_select(void)
{
	if (ImGui::BeginCombo(ICON_FA_FONT " Font Name", style_data.font.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Config::fonts); n++)
		{
			ImGui::PushID(n);
			std::string txt = Config::fonts[n];

			if (ImGui::Selectable(txt.c_str(), style_data.font == txt))
			{
				style_data.font = txt;
				style_data.changed = true;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	std::string txt_font = std::to_string(style_data.font_size);

	if (ImGui::BeginCombo(ICON_FA_TEXT_WIDTH " Font Size", txt_font.c_str()))
	{
		for (int n = MIN_FONT_SIZE; n <= MAX_FONT_SIZE; n++)
		{
			ImGui::PushID(n);
			std::string txt = std::to_string(n);

			if (ImGui::Selectable(txt.c_str(), n == style_data.font_size))
			{
				style_data.font_size = n;
				style_data.changed = true;
			}

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}

void Settings::draw_sidebar(void)
{
	Utils::center_text(ICON_FA_ANGLE_LEFT " Sidebar", true);

	if (ImGui::SliderInt("fade in", &sb_data.fade_in, 0, 5000))
	{
		Config::Sidebar_c::fade_in = sb_data.fade_in;
		sb_data.changed = true;
	}

	if (ImGui::SliderInt("fade out", &sb_data.fade_out, 0, 5000))
	{
		Config::Sidebar_c::fade_out = sb_data.fade_out;
		sb_data.changed = true;
	}
}

void Settings::draw_command_palette(void)
{
	Utils::center_text(ICON_FA_TERMINAL " Command Palette", true);

	int width = cp_data.size.x;
	int height = cp_data.size.y;

	if (ImGui::SliderInt("width", &width, 32, Config::win_width))
	{
		cp_data.size.x = width;
		Config::CommandPalette_c::size.x = width;
		cp_data.changed = true;
	}

	if (ImGui::SliderInt("height", &height, 32, Config::win_height))
	{
		cp_data.size.y = height;
		Config::CommandPalette_c::size.y = height;
		cp_data.changed = true;
	}
}

void Settings::draw_node_interface(void)
{
	Utils::center_text(ICON_FA_PROJECT_DIAGRAM " Node Interface", true);

	if (ImGui::ColorEdit3("Node Title Color", ni_data.title_color, ImGuiColorEditFlags_Float))
	{
		Config::NodeInterface_c::title_color.x = ni_data.title_color[0];
		Config::NodeInterface_c::title_color.y = ni_data.title_color[1];
		Config::NodeInterface_c::title_color.z = ni_data.title_color[2];
		ni_data.changed = true;
	}

	if (ImGui::ColorEdit3("Node Label Color", ni_data.label_color, ImGuiColorEditFlags_Float))
	{
		Config::NodeInterface_c::label_color.x = ni_data.label_color[0];
		Config::NodeInterface_c::label_color.y = ni_data.label_color[1];
		Config::NodeInterface_c::label_color.z = ni_data.label_color[2];
		ni_data.changed = true;
	}

	if (ImGui::SliderInt2("Node Frame Padding", ni_data.item_inner_spacing, 8, 64))
	{
		Config::NodeInterface_c::item_inner_spacing.x = ni_data.item_inner_spacing[0];
		Config::NodeInterface_c::item_inner_spacing.y = ni_data.item_inner_spacing[1];
		ni_data.changed = true;
	}
}

void Settings::draw_buttons(void)
{
	if (ImGui::Button(ICON_FA_CHECK " Save"))
	{
		if (style_data.changed)
			Config::update_style(style_data);

		if (cp_data.changed)
			Config::update_command_palette(cp_data);

		if (sb_data.changed)
			Config::update_sidebar(sb_data);

		if (ni_data.changed)
			Config::update_node_interface(ni_data);

		bool res = Config::save_user_config();
		PLOGV << "Save user config: " << res;

		if (res == RES_SUCCESS)
			Alert::open(ALERT_TYPE::SUCCESS, "Configurations saved! Please restart to see the changes.");
		else
			Alert::open(ALERT_TYPE::ERR, "We encountered an error! Sorry about that.");

		style_data.changed = false;
		cp_data.changed = false;
		sb_data.changed = false;
		ni_data.changed = false;
		was_changed = false;
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_UNDO " Restore Defaults"))
	{
		bool res = Config::reset();
		PLOGV << "Restore defaults: " << res;

		if (res == RES_SUCCESS)
			Alert::open(ALERT_TYPE::SUCCESS, "Configurations restored to defaults! Please restart to see the changes.");
		else
			Alert::open(ALERT_TYPE::ERR, "We encountered an error! Sorry about that.");

		Settings::reset_values();
		style_data.changed = false;
		cp_data.changed = false;
		sb_data.changed = false;
		ni_data.changed = false;
		was_changed = false;
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_TIMES " Close"))
		Settings::close();

	ImGui::SameLine();

	const char* icon = Settings::is_pos_locked ? ICON_FA_LOCK : ICON_FA_LOCK_OPEN;

	if (ImGui::Checkbox(icon, &Settings::is_pos_locked))
	{
		if (Settings::is_pos_locked)
			Settings::flags |= ImGuiWindowFlags_NoMove;
		else
			Settings::flags &= ~ImGuiWindowFlags_NoMove;
	}

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Lock/Unlock the position of this window");
}

void Settings::reset_values(void)
{
	ni_data.item_inner_spacing[0] = ni_data.item_inner_spacing_orig[0];
	ni_data.item_inner_spacing[1] = ni_data.item_inner_spacing_orig[1];
	ni_data.title_color[0] = ni_data.title_color_orig[0];
	ni_data.title_color[1] = ni_data.title_color_orig[1];
	ni_data.title_color[2] = ni_data.title_color_orig[2];
	ni_data.label_color[0] = ni_data.label_color_orig[0];
	ni_data.label_color[1] = ni_data.label_color_orig[1];
	ni_data.label_color[2] = ni_data.label_color_orig[2];

	Config::NodeInterface_c::title_color.x = ni_data.title_color_orig[0];
	Config::NodeInterface_c::title_color.y = ni_data.title_color_orig[1];
	Config::NodeInterface_c::title_color.z = ni_data.title_color_orig[2];
	Config::NodeInterface_c::label_color.x = ni_data.label_color_orig[0];
	Config::NodeInterface_c::label_color.y = ni_data.label_color_orig[1];
	Config::NodeInterface_c::label_color.z = ni_data.label_color_orig[2];
	Config::NodeInterface_c::item_inner_spacing.x = ni_data.item_inner_spacing_orig[0];
	Config::NodeInterface_c::item_inner_spacing.y = ni_data.item_inner_spacing_orig[1];

	was_changed = false;
}
}
