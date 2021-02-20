#include "font.hpp"

#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "imgui.h"
#include "defines.hpp"
#include "config.hpp"

const char* far_path = "assets/fonts/fa-regular-400.ttf";
const char* fas_path = "assets/fonts/fa-solid-900.ttf";
const char* fab_path = "assets/fonts/fa-brands-400.ttf";
const char* proggy_clean_path = "assets/fonts/ProggyClean.ttf";

namespace CodeNect
{
void Font::init()
{
	PLOGV << "Loading fonts...";

	ImGuiIO& io = ImGui::GetIO();

	if (Config::font_size >= MIN_FONT_SIZE && Config::font_size <= MAX_FONT_SIZE)
		io.Fonts->AddFontFromFileTTF(proggy_clean_path, Config::font_size);
	else
		io.Fonts->AddFontDefault();

	PLOGV << "Default fonts loaded successfully.";
	PLOGV << "Loading " << fas_path << " ...";

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	config.PixelSnapH = true;
	config.GlyphMinAdvanceX = Config::font_size;
	io.Fonts->AddFontFromFileTTF(fas_path, Config::font_size, &config, icon_ranges);

	PLOGV << fas_path << " loaded successfully";
}
}
