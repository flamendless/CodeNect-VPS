#include "sidebar_indicator.hpp"

#include "plog/Log.h"
#include "modules/filesystem.hpp"
#include "core/config.hpp"

namespace CodeNect
{
int SidebarIndicator::init(void)
{
	CodeNect::Image *img = new CodeNect::Image();
	const std::string filename = Config::Sidebar_c::indicator_filename;
	const int res = CodeNect::Filesystem::load_texture_from_file(filename.c_str(), *img);

	if (res == RES_FAIL)
	{
		PLOGE << "Failed to load texture from file: " << filename;
		return RES_FAIL;
	}

	m_btn.m_normal = img;

	return RES_SUCCESS;
}

void SidebarIndicator::set_style(void)
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
}

void SidebarIndicator::unset_style(void)
{
	ImGui::PopStyleVar(4);
}

void SidebarIndicator::draw(void)
{
	SidebarIndicator::set_style();
	ImVec2 center_pos(Config::Sidebar_c::pos.x, ImGui::GetIO().DisplaySize.y * 0.5);
	ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));
	ImGui::SetNextWindowSize(Config::Sidebar_c::indicator_size, ImGuiCond_Always);
	ImGui::Begin("SidebarIndicator", &m_is_open, m_flags);
		m_btn.draw();
	ImGui::End();
	SidebarIndicator::unset_style();
}
}
