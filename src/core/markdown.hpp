#ifndef _MARKDOWN_HPP
#define _MARKDOWN_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "imgui_markdown.h"
#include "core/font.hpp"
#include "core/utils.hpp"

namespace CodeNect::Markdown
{
inline void md_format_cb(const ImGui::MarkdownFormatInfo& md_info, bool start)
{
	ImGui::defaultMarkdownFormatCallback(md_info, start);
}

inline void md_link_cb(ImGui::MarkdownLinkCallbackData data)
{
	std::string url(data.link, data.linkLength);
	if (!data.isImage)
		Utils::open_url(url.c_str());
}

inline void draw(const std::string& str_md)
{
	Font::md_config.linkCallback = Markdown::md_link_cb;
	Font::md_config.tooltipCallback = NULL;
	Font::md_config.imageCallback = NULL;
	Font::md_config.linkIcon = ICON_FA_LINK;
	Font::md_config.headingFormats[0] = { Font::H1, true };
	Font::md_config.headingFormats[1] = { Font::H2, true };
	Font::md_config.headingFormats[2] = { Font::H3, false };
	Font::md_config.userData = NULL;
	Font::md_config.formatCallback = Markdown::md_format_cb;
	ImGui::Markdown(str_md.c_str(), str_md.length(), Font::md_config);
}
}

#endif //_MARKDOWN_HPP
