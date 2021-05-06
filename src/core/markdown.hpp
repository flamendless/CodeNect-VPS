#ifndef _MARKDOWN_HPP
#define _MARKDOWN_HPP

#include "IconsFontAwesome5.h"
#include "imgui.h"
#include "imgui_markdown.h"
#include "core/font.hpp"
#include "core/utils.hpp"

namespace CodeNect::Markdown
{
struct MD
{
	static ImFont* H1;
	static ImFont* H2;
	static ImFont* H3;
	static ImGui::MarkdownConfig md_config;
};

void md_format_cb(const ImGui::MarkdownFormatInfo& md_info, bool start);
void md_link_cb(ImGui::MarkdownLinkCallbackData data);
void md_tooltip_cb(ImGui::MarkdownTooltipCallbackData data);
void draw(const std::string& str_md);
}

#endif //_MARKDOWN_HPP
