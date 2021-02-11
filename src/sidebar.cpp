#include "sidebar.hpp"

namespace CodeNect
{
// map_images Sidebar::images;
// map_images Sidebar::images_hover;
map_ui_buttons Sidebar::ui_buttons;
map_tooltips Sidebar::tooltips {
	{"project", "Project/File"},
	{"run", "Run"},
	{"settings", "Settings"},
};

ImGuiWindowFlags Sidebar::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoFocusOnAppearing;

bool Sidebar::is_open = true;
bool Sidebar::has_clicked = false;
float Sidebar::alpha = 0.0f;

bool show_project = false;
ImGuiWindowFlags popup_flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse;

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
		PPK_ASSERT(Sidebar::tooltips[key] != "", "Tooltip does not exists for '%s'", key.c_str());
		ui_button.tooltip = Sidebar::tooltips[key];
		Sidebar::ui_buttons.insert(std::pair<std::string, CodeNect::UI_Button>(key, ui_button));

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
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Sidebar::alpha);
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
	Sidebar::draw_popup_project();
}

void Sidebar::draw_sidebar()
{
	CodeNect::UI_Button& btn_project = Sidebar::ui_buttons["project"];
	CodeNect::UI_Button& btn_run = Sidebar::ui_buttons["run"];
	CodeNect::UI_Button& btn_settings = Sidebar::ui_buttons["settings"];

	const ImVec2& max_size = Sidebar_c::max_img_size;

	Sidebar::set_style();
	ImGui::SetNextWindowPos(Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(Sidebar_c::size, ImGuiCond_Always);
	ImGui::Begin("Sidebar", &Sidebar::is_open, Sidebar::flags);
		btn_project.draw();

		if (btn_project.is_clicked)
			show_project = true;

		btn_run.draw();

		const int isy = CodeNect::Config::Sidebar_c::item_spacing.y;
		const int y = ImGui::GetContentRegionAvail().y - (max_size.y * 2) - (isy * 2);
		ImGui::Dummy(ImVec2(max_size.x, y));
		btn_settings.draw();
	ImGui::End();
	Sidebar::unset_style();

	Sidebar::has_clicked = btn_project.is_open ||
		btn_run.is_open ||
		btn_settings.is_open;
}

void Sidebar::draw_popup_project()
{
	CodeNect::UI_Button& btn_project = Sidebar::ui_buttons["project"];
	btn_project.is_open = show_project;

	if (btn_project.is_open)
	{
		bool is_inside = false;

		ImGui::Begin("Project", &show_project, popup_flags);
			is_inside = ImGui::IsWindowHovered();

			if (ImGui::BeginMenu("Project"))
			{
				ImGui::MenuItem("New Project");
				ImGui::MenuItem("Open Project");
				ImGui::MenuItem("Save Project");

            	ImGui::EndMenu();
			}
		ImGui::End();

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !is_inside)
			show_project = false;
	}
}
}
