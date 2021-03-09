#include "core/font.hpp"

#include "IconsFontAwesome5.h"
#include "plog/Log.h"
#include "core/config.hpp"

namespace CodeNect
{
const char* Font::far_path = "assets/fonts/fa-regular-400.ttf";
const char* Font::fas_path = "assets/fonts/fa-solid-900.ttf";
const char* Font::fab_path = "assets/fonts/fa-brands-400.ttf";
const char* Font::proggy_clean_path = "assets/fonts/ProggyClean.ttf";
const char* Font::proggy_tiny_path = "assets/fonts/ProggyTiny.ttf";
const char* Font::droid_sans_path = "assets/fonts/DroidSans.ttf";
const char* Font::cousine_regular_path = "assets/fonts/Cousine-Regular.ttf";
const char* Font::karla_regular_path = "assets/fonts/Karla-Regular.ttf";
const char* Font::roboto_medium_path = "assets/fonts/Roboto-Medium.ttf";
map_fonts Font::fonts;

void Font::init(void)
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

	Font::init_fonts();

	PLOGI << fas_path << " fontawesome loaded successfully";
}

void Font::init_fonts(void)
{
	PLOGI << "Loading custom fonts...";

	Font::fonts.insert(std::pair<const std::string, CustomFont*>("large", new CustomFont(16.0f)));
	Font::fonts.insert(std::pair<const std::string, CustomFont*>("extra_large", new CustomFont(24.0f)));

	ImGuiIO& io = ImGui::GetIO();

	for (std::pair<const std::string, CustomFont*>& value : Font::fonts)
	{
		CustomFont* font = value.second;
		float size = font->size;

		PLOGV << "Loading: " << value.first << " of size: " << size << "...";

		font->font = io.Fonts->AddFontFromFileTTF(Font::roboto_medium_path, size);
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;
		config.GlyphMinAdvanceX = size;
		io.Fonts->AddFontFromFileTTF(Font::fas_path, size, &config, icon_ranges);

		PLOGV << "Loaded: " << value.first << " of size: " << size << " successfully";
	}

	PLOGI << "Loaded custom fonts successfully";
}

void Font::use_font(FONT_SIZE size)
{
	CustomFont* font;

	switch(size)
	{
		case FONT_SIZE::LARGE:
		{
			font = Font::fonts["large"];
			break;
		}
		case FONT_SIZE::EXTRA_LARGE:
		{
			font = Font::fonts["extra_large"];
			break;
		}
	}

	ImGui::PushFont(font->font);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(font->size, font->size));
}

void Font::unuse_font(void)
{
	ImGui::PopStyleVar();
	ImGui::PopFont();
}
}