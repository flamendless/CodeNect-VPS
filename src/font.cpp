#include "font.hpp"

const char* far_path = "assets/fonts/fa-regular-400.ttf";
const char* fas_path = "assets/fonts/fa-solid-900.ttf";
const char* fab_path = "assets/fonts/fa-brands-400.ttf";

namespace CodeNect
{
void Font::init()
{
	PLOGV << "Loading fonts...";

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	PLOGV << "Default font loaded successfully.";
	PLOGV << "Loading " << fas_path << " ...";

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	config.PixelSnapH = true;
	config.GlyphMinAdvanceX = 13.0f;
	io.Fonts->AddFontFromFileTTF(fas_path, 13.0f, &config, icon_ranges);

	PLOGV << fas_path << " loaded successfully";
}
}
