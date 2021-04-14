#ifndef _FILESYSTEM_HPP
#define _FILESYSTEM_HPP

#include <string>
#include "core/image.hpp"

namespace CodeNect::Filesystem
{
bool open_project_file(std::string& project_filepath);
bool open_filepath(std::string& filepath);
bool save_file(std::string& out_filepath, const char* ext, const std::string& content);
int load_texture_from_file(const char* filename, CodeNect::Image& image);
}

#endif //_FILESYSTEM_HPP
