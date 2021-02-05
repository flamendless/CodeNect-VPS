#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <map>
#include <SDL_opengl.h>
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
