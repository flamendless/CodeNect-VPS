#include "sidebar.hpp"

namespace CodeNect
{
PopupProject popup_project;
PopupRun popup_run;

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

	return RES_SUCCESS;
}

int Sidebar::load_images()
{
	PLOGV << "Loading sidebar images...";

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
		CodeNect::UI_Button ui_button;
		ui_button.normal = image;
		ui_button.hovered = image_hover;
		ui_button.hoverable = true;
		PPK_ASSERT(m_tooltips[key] != "", "Tooltip does not exists for '%s'", key.c_str());
		ui_button.tooltip = m_tooltips[key];
		m_ui_buttons.insert(std::pair<std::string, CodeNect::UI_Button>(key, ui_button));

		// Sidebar::images.insert(std::pair<std::string, CodeNect::Image>(key, image));
		// Sidebar::images_hover.insert(std::pair<std::string, CodeNect::Image>(key_hover, image_hover));
	}

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

void Sidebar::draw()
{
	Sidebar::draw_sidebar();

	CodeNect::UI_Button& btn_project = m_ui_buttons["project"];
	CodeNect::UI_Button& btn_run = m_ui_buttons["run"];
	// CodeNect::UI_Button& btn_settings = Sidebar::ui_buttons["settings"];

	btn_project.is_open = popup_project.is_open;
	if (btn_project.is_open) popup_project.draw();

	btn_run.is_open = popup_run.is_open;
	if (btn_run.is_open) popup_run.draw();
}

void Sidebar::draw_sidebar()
{
	CodeNect::UI_Button& btn_project = m_ui_buttons["project"];
	CodeNect::UI_Button& btn_run = m_ui_buttons["run"];
	CodeNect::UI_Button& btn_settings = m_ui_buttons["settings"];

	const ImVec2& max_size = Sidebar_c::max_img_size;

	Sidebar::set_style();
	ImGui::SetNextWindowPos(Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(Sidebar_c::size, ImGuiCond_Always);
	ImGui::Begin("Sidebar", &m_is_open, m_flags);
		btn_project.draw();

		if (btn_project.is_clicked) popup_project.is_open = true;

		btn_run.draw();

		if (btn_run.is_clicked) popup_run.is_open = true;

		const int isy = CodeNect::Config::Sidebar_c::item_spacing.y;
		const int y = ImGui::GetContentRegionAvail().y - (max_size.y * 2) - (isy * 2);
		ImGui::Dummy(ImVec2(max_size.x, y));
		btn_settings.draw();
	ImGui::End();
	Sidebar::unset_style();

	m_has_clicked = btn_project.is_open ||
		btn_run.is_open ||
		btn_settings.is_open;
}
}
