#ifndef DEFINES_HPP
#define DEFINES_HPP

#define RES_SUCCESS 0
#define RES_FAIL -1

#define BUF_SIZE 256
#define PROJECT_EXT "cn"
#define PROJECT_EXT_DOT ".cn"

#define MIN_FONT_SIZE 11
#define MAX_FONT_SIZE 16
#define DEFAULT_FONT "ProggyClean"
#define DEFAULT_FONT_SIZE 13
#define DEFAULT_STYLE "dark"

#define RES_VARNAME_CONFLICT_KEYWORD -2
#define RES_VARNAME_INVALID -1
#define RES_VARNAME_VALID 1
#define STR_CONFLICT_KEYWORD "variable name can't be used because it's a reserved keyword"

#include <map>
#include <string>
#include <vector>
#include "core/image.hpp"
#include "ui/button.hpp"
#include "core/custom_font.hpp"

// typedef std::map<const std::string, CodeNect::Image> map_images;
typedef std::map<const std::string, CodeNect::Button*> map_ui_buttons;
typedef std::map<const std::string, const::std::string> map_tooltips;
typedef std::map<const std::string, CodeNect::CustomFont*> map_fonts;
typedef std::pair<std::string, std::string> pair_key_filename;
typedef std::vector<pair_key_filename> vec_filenames;

#endif //DEFINES_HPP
