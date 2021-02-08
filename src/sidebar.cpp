#include "sidebar.hpp"
#include "config.hpp"

namespace CodeNect
{
map_images Sidebar::sidebar_images;
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
	vec_filenames &images_filenames = CodeNect::Config::Sidebar_c::images_filenames;

	for (int i = 0; i < images_filenames.size(); i++)
	{
		const std::pair<const std::string, const std::string>& si = images_filenames[i];
		const std::string& key = si.first;
		const std::string& filename = si.second;
		CodeNect::Image image;

		const int res = CodeNect::Filesystem::load_texture_from_file(filename.c_str(), image);

		if (res == RES_FAIL)
		{
			PLOGE << "Failed to load texture from file: " << filename;
			return RES_FAIL;
		}

		Sidebar::sidebar_images.insert(std::pair<std::string, CodeNect::Image>(key, image));

		PLOGV << "Loaded texture from file: " << filename;
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
	ImGui::PopStyleVar(0);
}

void Sidebar::draw()
{
	CodeNect::Image& img = Sidebar::sidebar_images["folder"];
	CodeNect::Image& img2 = Sidebar::sidebar_images["play"];

	void* texture = img.get_texture();
	void* texture2 = img2.get_texture();

	//for faking animation
	if (img.is_hovered) texture = texture2;

	ImGui::SetNextWindowPos(CodeNect::Config::Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(CodeNect::Config::Sidebar_c::size, ImGuiCond_Always);
	Sidebar::set_style();
	ImGui::Begin("Sidebar", &Sidebar::is_open, Sidebar::flags);
		ImGui::ImageButton(texture, img.size, ImVec2(), ImVec2(1, 1), 0, ImVec4(1, 0, 0, 1.0f), ImVec4(1, 1, 1, 1));
		if (ImGui::IsItemHovered()) img.is_hovered = true;
		else img.is_hovered = false;

		ImGui::ImageButton(texture2, img2.size, ImVec2(), ImVec2(1, 1), 0, ImVec4(1, 0, 0, 1.0f), ImVec4(1, 1, 1, 1));

		const int isy = CodeNect::Config::Sidebar_c::item_spacing.y;
		const int y = ImGui::GetContentRegionAvail().y - (img.height * 2) - (isy * 2);
		ImGui::Dummy(ImVec2(img.width, y));

		ImGui::ImageButton(texture, img.size, ImVec2(), ImVec2(1, 1), 0, ImVec4(1, 0, 0, 1.0f), ImVec4(1, 1, 1, 1));
		ImGui::ImageButton(texture2, img2.size, ImVec2(), ImVec2(1, 1), 0, ImVec4(1, 0, 0, 1.0f), ImVec4(1, 1, 1, 1));
	ImGui::End();
	Sidebar::unset_style();
}
}
