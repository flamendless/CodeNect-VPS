#include "core/markdown.hpp"

namespace CodeNect::Markdown
{
ImFont* MD::H1;
ImFont* MD::H2;
ImFont* MD::H3;
ImGui::MarkdownConfig MD::md_config;

void md_format_cb(const ImGui::MarkdownFormatInfo& md_info, bool start)
{
	ImGui::defaultMarkdownFormatCallback(md_info, start);
	switch (md_info.type)
	{
		case ImGui::MarkdownFormatType::EMPHASIS:
		{
			if (md_info.level == 1)
			{
				//normal emphasis *foo*
				if (start)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
				else
					ImGui::PopStyleColor();
			}
			break;
		}
    }
}

void md_link_cb(ImGui::MarkdownLinkCallbackData data)
{
	std::string url(data.link, data.linkLength);
	if (!data.isImage)
		Utils::open_url(url.c_str());
}

void md_tooltip_cb(ImGui::MarkdownTooltipCallbackData data)
{
	std::string url(data.linkData.link, data.linkData.linkLength);
	ImGui::SetTooltip("Open URL in your browser");
}

void draw(const std::string& str_md)
{
	MD::md_config.linkCallback = md_link_cb;
	MD::md_config.tooltipCallback = md_tooltip_cb;
	MD::md_config.imageCallback = NULL;
	MD::md_config.linkIcon = ICON_FA_LINK;
	MD::md_config.headingFormats[0] = { MD::H1, true };
	MD::md_config.headingFormats[1] = { MD::H2, true };
	MD::md_config.headingFormats[2] = { MD::H3, false };
	MD::md_config.userData = NULL;
	MD::md_config.formatCallback = md_format_cb;
	ImGui::Markdown(str_md.c_str(), str_md.length(), MD::md_config);
}
}
