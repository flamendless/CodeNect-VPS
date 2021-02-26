#ifndef DEFINES_HPP
#define DEFINES_HPP

#define RES_SUCCESS 0
#define RES_FAIL -1

#define PROJECT_EXT "cn"

#define MIN_FONT_SIZE 11
#define MAX_FONT_SIZE 16
#define DEFAULT_FONT "ProggyClean"
#define DEFAULT_FONT_SIZE 13
#define DEFAULT_STYLE "dark"

#include <map>
#include <string>
#include <vector>
#include "image.hpp"
#include "ui_button.hpp"

typedef std::map<const std::string, CodeNect::Image> map_images;
typedef std::map<const std::string, CodeNect::UI_Button> map_ui_buttons;
typedef std::map<const std::string, const::std::string> map_tooltips;
typedef std::pair<std::string, std::string> pair_key_filename;
typedef std::vector<pair_key_filename> vec_filenames;

#endif //DEFINES_HPP
