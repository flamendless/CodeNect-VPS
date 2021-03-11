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
#include "ui/help.hpp"

namespace CodeNect
{
Button* btn_project;
Button* btn_run;
Button* btn_settings;
Button* btn_about;
Button* btn_help;

PopupProject popup_project;
PopupRun popup_run;

int Sidebar::init(void)
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

	btn_project = m_ui_buttons["project"];
	btn_run = m_ui_buttons["run"];
	btn_settings = m_ui_buttons["settings"];
	btn_about = m_ui_buttons["about"];
	btn_help = m_ui_buttons["help"];

	return RES_SUCCESS;
}

int Sidebar::load_images(void)
{
	PLOGI << "Loading sidebar images...";

	vec_filenames& images_filenames = Config::Sidebar_c::images_filenames;
	vec_filenames& images_filenames_hover = Config::Sidebar_c::images_filenames_hover;

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
		CodeNect::Button* btn = new CodeNect::Button(image, image_hover, true);
		m_ui_buttons.insert(std::pair<std::string, CodeNect::Button*>(key, btn));

		// Sidebar::images.insert(std::pair<std::string, CodeNect::Image>(key, image));
		// Sidebar::images_hover.insert(std::pair<std::string, CodeNect::Image>(key_hover, image_hover));
	}

	PLOGI << "Loaded sidebar images successfully";

	return RES_SUCCESS;
}

void Sidebar::check_image_size(const CodeNect::Image& img)
{
	const ImVec2& ms = Config::Sidebar_c::max_img_size;
	const ImVec2& is = img.size;

	std::string err = fmt::format("({}, {}) != ({}, {})", is.x, is.y, ms.x, ms.y);
	PPK_ASSERT((is.x == ms.x && is.y == ms.y), "%s", err.c_str());
}

void Sidebar::set_style(void)
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Config::Sidebar_c::padding);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Config::Sidebar_c::item_spacing);
}

void Sidebar::unset_style(void)
{
	ImGui::PopStyleVar(4);
}

void Sidebar::manage_popups(void)
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

	if (Settings::is_open)
	{
		btn_project->m_hoverable = false;
		btn_run->m_hoverable = false;
		btn_settings->m_hoverable = true;
		btn_about->m_hoverable = false;
	}
	else if (About::is_open)
	{
		btn_project->m_hoverable = false;
		btn_run->m_hoverable = false;
		btn_settings->m_hoverable = false;
		btn_about->m_hoverable = true;
	}
	else if (Help::is_open)
	{
		btn_project->m_hoverable = false;
		btn_run->m_hoverable = false;
		btn_settings->m_hoverable = false;
		btn_about->m_hoverable = false;
		btn_help->m_hoverable = true;
	}
	else
	{
		btn_project->m_hoverable = true;
		btn_run->m_hoverable = true;
		btn_settings->m_hoverable = true;
		btn_about->m_hoverable = true;
	}
}

void Sidebar::draw(void)
{
	Sidebar::draw_sidebar();

	btn_project->m_is_open = popup_project.m_is_open;
	if (btn_project->m_is_open) popup_project.draw();

	btn_run->m_is_open = popup_run.m_is_open;
	if (btn_run->m_is_open) popup_run.draw();

	btn_settings->m_is_open = Settings::is_open;
	btn_about->m_is_open = About::is_open;
	btn_help->m_is_open = Help::is_open;

	m_has_opened = btn_project->m_is_open ||
		btn_run->m_is_open ||
		btn_settings->m_is_open ||
		btn_about->m_is_open ||
		btn_help->m_is_open;
}

void Sidebar::draw_sidebar(void)
{
	const ImVec2& max_size = Config::Sidebar_c::max_img_size;

	Sidebar::set_style();
	ImGui::SetNextWindowPos(Config::Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(Config::Sidebar_c::size);
	ImGui::Begin("Sidebar", &m_is_open, m_flags);
		popup_project.m_pos = ImGui::GetCursorPos();
		btn_project->draw();

		popup_run.m_pos = ImGui::GetCursorPos();
		btn_run->draw();

		static const int btn_after = 3;
		const int isy = CodeNect::Config::Sidebar_c::item_spacing.y;
		const int y = ImGui::GetContentRegionAvail().y - (max_size.y * btn_after) - (isy * btn_after);
		ImGui::Dummy(ImVec2(max_size.x, y));

		if (ImGui::IsItemHovered())
		{
			popup_project.m_is_open = false;
			popup_run.m_is_open = false;
		}

		btn_settings->draw();
		if (!m_has_opened && btn_settings->m_is_clicked)
			Settings::open();

		btn_about->draw();
		if (!m_has_opened && btn_about->m_is_clicked)
			About::open();

		btn_help->draw();
		if (!m_has_opened && btn_help->m_is_clicked)
			Help::open();

		Sidebar::manage_popups();
	ImGui::End();
	Sidebar::unset_style();

	Settings::draw();
	About::draw();
	Help::draw();
}

void Sidebar::shutdown()
{
	PLOGI << "Clearing sidebar buttons...";

	m_ui_buttons.clear();

	PLOGI << "Cleared sidebar buttons successfully";
}
}
