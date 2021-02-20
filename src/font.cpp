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
const char* proggy_tiny_path = "assets/fonts/ProggyTiny.ttf";
const char* droid_sans_path = "assets/fonts/DroidSans.ttf";
const char* cousine_regular_path = "assets/fonts/Cousine-Regular.ttf";
const char* karla_regular_path = "assets/fonts/Karla-Regular.ttf";
const char* roboto_medium_path = "assets/fonts/Roboto-Medium.ttf";

namespace CodeNect
{
void Font::init()
{
	PLOGI << "Loading fonts...";

	ImGuiIO& io = ImGui::GetIO();

	const char* font_path;

	if (Config::font == "ProggyClean")
		font_path = proggy_clean_path;
	else if (Config::font == "ProggyTiny")
		font_path = proggy_tiny_path;
	else if (Config::font == "DroidSans")
		font_path = droid_sans_path;
	else if (Config::font == "CousineRegular")
		font_path = cousine_regular_path;
	else if (Config::font == "KarlaRegular")
		font_path = karla_regular_path;
	else if (Config::font == "RobotoMedium")
		font_path = roboto_medium_path;

	if (Config::font_size >= MIN_FONT_SIZE && Config::font_size <= MAX_FONT_SIZE)
		io.Fonts->AddFontFromFileTTF(font_path, Config::font_size);
	else
		io.Fonts->AddFontDefault();

	PLOGI << "Default fonts loaded successfully";
	PLOGI << "Loading fontawesome: " << fas_path << " ...";

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	config.PixelSnapH = true;
	config.GlyphMinAdvanceX = Config::font_size;
	io.Fonts->AddFontFromFileTTF(fas_path, Config::font_size, &config, icon_ranges);

	PLOGI << fas_path << " fontawesome loaded successfully";
}
}
