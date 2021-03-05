#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include "image.hpp"

namespace CodeNect
{
namespace Filesystem
{
bool open_project_file(std::string& project_filepath);
bool open_filepath(std::string& filepath);
int load_texture_from_file(const char* filename, CodeNect::Image& image);
}
}

#endif //FILESYSTEM_HPP
