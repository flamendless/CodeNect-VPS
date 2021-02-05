#include "sidebar.hpp"
#include "config.hpp"

namespace CodeNect
{
map_images Sidebar::sidebar_images;
ImGuiWindowFlags Sidebar::flags = ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
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

void Sidebar::draw()
{
	CodeNect::Image& img = Sidebar::sidebar_images["test"];
	// CodeNect::Image& img2 = Sidebar::sidebar_images["test2"];
	void* texture = img.get_texture();
	// void* texture2 = img2.get_texture();
    //
	// ImGui::Begin("OpenGL Texture Text");
	// ImGui::Text("pointer = %p", texture);
	// ImGui::Text("pointer = %p", texture2);
	// ImGui::End();

	int &h = CodeNect::Config::win_height;

	ImGui::SetNextWindowPos(ImVec2(CodeNect::Config::Sidebar_c::pad_x, (float)h/2), ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(128, 128), ImGuiCond_Always);
	ImGui::Begin("Sidebar", &Sidebar::is_open, Sidebar::flags);
		ImGui::Text("test");
		// ImGui::Image(texture, ImVec2(img.width, img.height));
	ImGui::End();
}
}
