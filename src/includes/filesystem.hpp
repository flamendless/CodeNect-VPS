#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <GLFW/glfw3.h>
#include <map>
#include "nfd.h"
#include "fmt/printf.h"
#include "plog/Log.h"
#include "stb_image.h"
#include "defines.hpp"

namespace CodeNect
{
namespace Filesystem
{
static const char* filter_project = "md";

bool open_project_file(std::string& project_filepath);
int load_texture_from_file(const char* filename, CodeNect::Image& image);
}
}

#endif //FILESYSTEM_H
