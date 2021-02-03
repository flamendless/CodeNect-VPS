#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SDL_opengl.h>
#include "nfd.h"
#include "fmt/printf.h"
#include "plog/Log.h"
#include "stb_image.h"

namespace Filesystem
{
static const char* filter_project = "md";

bool open_project_file(std::string& project_filepath);
bool load_texture_from_file(const char* filename, GLuint* out, int* width, int* height);
bool load_images(const std::vector<std::string>& filenames);
}

#endif //FILESYSTEM_H
