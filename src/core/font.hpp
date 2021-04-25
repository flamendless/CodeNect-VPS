#ifndef _FONT_HPP
#define _FONT_HPP

#include <string>
#include <map>
#include "imgui.h"
#include "imgui_markdown.h"
#include "core/defines.hpp"

namespace CodeNect
{
enum FONT_SIZE { LARGE, EXTRA_LARGE };

struct Font
{
	static const char* far_path;
	static const char* fas_path;
	static const char* fab_path;
	static const char* proggy_clean_path;
	static const char* proggy_tiny_path;
	static const char* droid_sans_path;
	static const char* cousine_regular_path;
	static const char* karla_regular_path;
	static const char* roboto_medium_path;
	static const char* md_path;
	static const char* md_bold_path;
	static map_fonts fonts;

	//markdown
	static ImFont* H1;
	static ImFont* H2;
	static ImFont* H3;
	static ImGui::MarkdownConfig md_config;

	Font() = delete;
	static void init(void);
	static void init_fonts(void);
	static void init_md_fonts(void);
	static CustomFont* get_custom_font(FONT_SIZE size);
	static void use_font(FONT_SIZE size);
	static float get_font_size(FONT_SIZE size);
	static void unuse_font(void);
	static void shutdown(void);
};
}

#endif //_FONT_HPP
