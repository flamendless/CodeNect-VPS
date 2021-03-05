#include "sidebar.hpp"

#include <vector>
#include "fmt/core.h"
#include "plog/Log.h"
#include "ppk_assert.h"
#include "modules/filesystem.hpp"
#include "core/config.hpp"
#include "ui/button.hpp"
#include "popups/popup.hpp"
#include "ui/settings.hpp"
#include "ui/about.hpp"

namespace CodeNect
{
Button* btn_project;
Button* btn_run;
Button* btn_settings;
Button* btn_about;

PopupProject popup_project;
PopupRun popup_run;
Settings settings;
About about;

int Sidebar::init()
{
	if (Sidebar::load_images() != RES_SUCCESS) return RES_FAIL;

	// for (auto const& value : sidebar_images)
	// for (std::pair<const std::string, const CodeNect::Image>& value : Sidebar::sidebar_images)
	// {
	// 	const std::string& key = value.first;
	// 	const CodeNect::Image& image = value.second;
    //
	// 	PLOGV << key << " = "  << image.texture;
	// }

	btn_project = &m_ui_buttons["project"];
	btn_run = &m_ui_buttons["run"];
	btn_settings = &m_ui_buttons["settings"];
	btn_about = &m_ui_buttons["about"];

	popup_project.init();
	settings.init();

	return RES_SUCCESS;
}

int Sidebar::load_images()
{
	PLOGI << "Loading sidebar images...";

	vec_filenames& images_filenames = Sidebar_c::images_filenames;
	vec_filenames& images_filenames_hover = Sidebar_c::images_filenames_hover;

	for (int i = 0; i < images_filenames.size(); i++)
	{
		CodeNect::Image* image = new CodeNect::Image();
		const std::pair<const std::string, const std::string>& p = images_filenames[i];
		const std::string& key = p.first;
		const std::string& filename = p.second;
		const int res = CodeNect::Filesystem::load_texture_from_file(filename.c_str(), *image);

		if (res == RES_FAIL)
		{
			PLOGE << "Failed to load texture from file: " << filename;
			return RES_FAIL;
		}

		Sidebar::check_image_size(*image);
		PLOGV << "Loaded successfully: " << key;

		//load hover
		CodeNect::Image* image_hover = new CodeNect::Image();
		const std::pair<const std::string, const std::string>& p2 = images_filenames_hover[i];
		const std::string& key_hover = p2.first;
		const std::string& filename_hover = p2.second;
		const int res_hover = CodeNect::Filesystem::load_texture_from_file(filename_hover.c_str(), *image_hover);

		if (res_hover == RES_FAIL)
		{
			PLOGE << "Failed to load texture from file: " << filename;
			return RES_FAIL;
		}

		Sidebar::check_image_size(*image_hover);
		PLOGV << "Loaded successfully: " << key_hover;

		//create ui button
		CodeNect::Button ui_button;
		ui_button.m_normal = image;
		ui_button.m_hovered = image_hover;
		ui_button.m_hoverable = true;
		m_ui_buttons.insert(std::pair<std::string, CodeNect::Button>(key, ui_button));

		// Sidebar::images.insert(std::pair<std::string, CodeNect::Image>(key, image));
		// Sidebar::images_hover.insert(std::pair<std::string, CodeNect::Image>(key_hover, image_hover));
	}

	PLOGI << "Loaded sidebar images successfully";

	return RES_SUCCESS;
}

void Sidebar::check_image_size(const CodeNect::Image& img)
{
	const ImVec2& ms = Sidebar_c::max_img_size;
	const ImVec2& is = img.size;

	std::string err = fmt::format("({}, {}) != ({}, {})", is.x, is.y, ms.x, ms.y);
	PPK_ASSERT((is.x == ms.x && is.y == ms.y), "%s", err.c_str());
}

void Sidebar::set_style()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Sidebar_c::padding);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Sidebar_c::item_spacing);
}

void Sidebar::unset_style()
{
	ImGui::PopStyleVar(4);
}

void Sidebar::update(float dt)
{
}

void Sidebar::manage_popups()
{
	if (btn_project->m_is_clicked)
	{
		popup_project.m_is_open = true;
		popup_run.m_is_open = false;
	}
	else if (btn_run->m_is_clicked)
	{
		popup_project.m_is_open = false;
		popup_run.m_is_open = true;
	}
	else if (btn_settings->m_is_clicked)
	{
		popup_project.m_is_open = false;
		popup_run.m_is_open = false;
	}
	else if (btn_about->m_is_clicked)
	{
		popup_project.m_is_open = false;
		popup_run.m_is_open = false;
	}

	if (settings.m_is_open)
	{
		btn_project->m_hoverable = false;
		btn_run->m_hoverable = false;
		btn_settings->m_hoverable = true;
		btn_about->m_hoverable = false;
	}
	else if (about.m_is_open)
	{
		btn_project->m_hoverable = false;
		btn_run->m_hoverable = false;
		btn_settings->m_hoverable = false;
		btn_about->m_hoverable = true;
	}
	else
	{
		btn_project->m_hoverable = true;
		btn_run->m_hoverable = true;
		btn_settings->m_hoverable = true;
		btn_about->m_hoverable = true;
	}
}

void Sidebar::draw()
{
	Sidebar::draw_sidebar();

	btn_project->m_is_open = popup_project.m_is_open;
	if (btn_project->m_is_open) popup_project.draw();

	btn_run->m_is_open = popup_run.m_is_open;
	if (btn_run->m_is_open) popup_run.draw();

	btn_settings->m_is_open = settings.m_is_open;
	settings.draw();

	btn_about->m_is_open = about.m_is_open;
	about.draw();

	m_has_opened = btn_project->m_is_open ||
		btn_run->m_is_open ||
		btn_settings->m_is_open ||
		btn_about->m_is_open;
}

void Sidebar::draw_sidebar()
{
	const ImVec2& max_size = Sidebar_c::max_img_size;

	Sidebar::set_style();
	ImGui::SetNextWindowPos(Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(Sidebar_c::size);
	ImGui::Begin("Sidebar", &m_is_open, m_flags);
		popup_project.m_pos = ImGui::GetCursorPos();
		btn_project->draw();

		popup_run.m_pos = ImGui::GetCursorPos();
		btn_run->draw();

		const int isy = CodeNect::Config::Sidebar_c::item_spacing.y;
		const int y = ImGui::GetContentRegionAvail().y - (max_size.y * 2) - (isy * 2);
		ImGui::Dummy(ImVec2(max_size.x, y));

		if (ImGui::IsItemHovered())
		{
			popup_project.m_is_open = false;
			popup_run.m_is_open = false;
		}

		btn_settings->draw();
		if (!m_has_opened && btn_settings->m_is_clicked)
			settings.m_is_open = true;

		btn_about->draw();
		if (!m_has_opened && btn_about->m_is_clicked)
			about.m_is_open = true;

		Sidebar::manage_popups();
	ImGui::End();
	Sidebar::unset_style();
}
}
