#include "sidebar_indicator.hpp"

namespace CodeNect
{
int SidebarIndicator::init()
{
	CodeNect::Image *img = new CodeNect::Image();
	const std::string filename = Sidebar_c::indicator_filename;
	const int res = CodeNect::Filesystem::load_texture_from_file(filename.c_str(), *img);

	if (res == RES_FAIL)
	{
		PLOGE << "Failed to load texture from file: " << filename;
		return RES_FAIL;
	}

	m_btn.normal = img;

	return RES_SUCCESS;
}

void SidebarIndicator::set_style()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
}

void SidebarIndicator::unset_style()
{
	ImGui::PopStyleVar(4);
}

void SidebarIndicator::update(float dt)
{
}

void SidebarIndicator::draw()
{

	SidebarIndicator::set_style();
	ImGui::SetNextWindowPos(Sidebar_c::pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(Sidebar_c::indicator_size, ImGuiCond_Always);
	ImGui::Begin("SidebarIndicator", &m_is_open, m_flags);
		m_btn.draw();
	ImGui::End();
	SidebarIndicator::unset_style();
}
}
