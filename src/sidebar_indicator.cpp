#include "sidebar_indicator.hpp"

namespace CodeNect
{
ImVec2 SidebarIndicator::cur_pos;
ImVec2 SidebarIndicator::size;
ImGuiWindowFlags SidebarIndicator::flags =
	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground |
	ImGuiWindowFlags_NoFocusOnAppearing;
bool SidebarIndicator::is_open = true;
bool SidebarIndicator::is_hovered = false;
CodeNect::UI_Button SidebarIndicator::btn;

int SidebarIndicator::init()
{
	SidebarIndicator::size = ImVec2(64, 64);
	SidebarIndicator::cur_pos = ImVec2(-SidebarIndicator::size.x, (int)(CodeNect::Config::win_height/2));

	CodeNect::Image *img = new CodeNect::Image();
	const std::string filename = Sidebar_c::indicator_filename;
	const int res = CodeNect::Filesystem::load_texture_from_file(filename.c_str(), *img);

	if (res == RES_FAIL)
	{
		PLOGE << "Failed to load texture from file: " << filename;
		return RES_FAIL;
	}

	SidebarIndicator::btn.normal = img;

	return RES_SUCCESS;
}

void SidebarIndicator::set_style()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
}

void SidebarIndicator::unset_style()
{
	ImGui::PopStyleVar(3);
}

void SidebarIndicator::update(float dt)
{
}

void SidebarIndicator::draw()
{

	SidebarIndicator::set_style();
	ImGui::SetNextWindowPos(SidebarIndicator::cur_pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(SidebarIndicator::size, ImGuiCond_Always);
	ImGui::Begin("SidebarIndicator", &SidebarIndicator::is_open, SidebarIndicator::flags);
		SidebarIndicator::btn.draw();

		SidebarIndicator::is_hovered = ImGui::IsWindowHovered();
	ImGui::End();
	SidebarIndicator::unset_style();
}
}
