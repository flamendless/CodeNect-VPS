#ifndef DEFINES_H
#define DEFINES_H

#define RES_SUCCESS 0
#define RES_FAIL -1

#include <map>
#include <string>
#include <vector>
#include "image.hpp"

typedef std::map<const std::string, CodeNect::Image> map_images;
typedef std::pair<std::string, std::string> pair_key_filename;
typedef std::vector<pair_key_filename> vec_filenames;

#endif //DEFINES_H
