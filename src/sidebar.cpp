#include "sidebar.hpp"
#include "config.hpp"

namespace CodeNect
{
map_images Sidebar::images;
map_images Sidebar::images_hover;
map_ui_buttons Sidebar::ui_buttons;
map_tooltips Sidebar::tooltips {
	{"folder", "Project/File"},
	{"run", "Run"},
};

ImGuiWindowFlags Sidebar::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse;
bool Sidebar::is_open = true;

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

	vec_filenames &images_filenames = CodeNect::Config::Sidebar_c::images_filenames;
	vec_filenames &images_filenames_hover = CodeNect::Config::Sidebar_c::images_filenames_hover;

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
		PLOGV << "Loaded successfully: " << key_hover;

		//create ui button
		CodeNect::UI_Button ui_button;
		ui_button.normal = image;
		ui_button.hovered = image_hover;
		ui_button.tooltip = Sidebar::tooltips[key];
		Sidebar::ui_buttons.insert(std::pair<std::string, CodeNect::UI_Button>(key, ui_button));

		// Sidebar::images.insert(std::pair<std::string, CodeNect::Image>(key, image));
		// Sidebar::images_hover.insert(std::pair<std::string, CodeNect::Image>(key_hover, image_hover));
	}

	return RES_SUCCESS;
}

void Sidebar::set_style()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, CodeNect::Config::Sidebar_c::padding);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, CodeNect::Config::Sidebar_c::item_spacing);
}

void Sidebar::unset_style()
{
	ImGui::PopStyleVar(3);
}

void Sidebar::draw()
{
	// CodeNect::Image& img_folder = Sidebar::sidebar_images["folder"];
	// CodeNect::Image& img_run = Sidebar::sidebar_images["run"];
	CodeNect::UI_Button& btn_folder = Sidebar::ui_buttons["folder"];
	CodeNect::UI_Button& btn_run = Sidebar::ui_buttons["run"];

	Sidebar::set_style();
	ImGui::SetNextWindowPos(CodeNect::Config::Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(CodeNect::Config::Sidebar_c::size, ImGuiCond_Always);
	ImGui::Begin("Sidebar", &Sidebar::is_open, Sidebar::flags);
		btn_folder.draw();
		btn_run.draw();

		// const int isy = CodeNect::Config::Sidebar_c::item_spacing.y;
		// const int y = ImGui::GetContentRegionAvail().y - (img_folder.height * 2) - (isy * 2);
		// ImGui::Dummy(ImVec2(img_folder.width, y));

		// ImGui::ImageButton(texture, img.size, ImVec2(), ImVec2(1, 1), 0, bg, ImVec4(1, 1, 1, 1));
		// ImGui::ImageButton(texture2, img2.size, ImVec2(), ImVec2(1, 1), 0, bg, ImVec4(1, 1, 1, 1));
	ImGui::End();
	Sidebar::unset_style();
}
}
