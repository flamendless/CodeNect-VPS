#ifndef _FILESYSTEM_HPP
#define _FILESYSTEM_HPP

#include <filesystem>
#include <string>
#include <GLFW/glfw3.h>
#include "core/image.hpp"

namespace CodeNect::Filesystem
{
struct Paths
{
	static std::filesystem::path out_dir;
};

bool open_project_file(std::string& project_filepath);
bool open_filepath(std::string& filepath);
bool save_to_file(std::string& out_filepath, const char* ext, const std::string& content);
void create_project_temp_dir(void);
int load_texture_from_file(const char* filename, CodeNect::Image& image);
unsigned char* load_texture_from_file(const char* filename, GLFWimage& image);

#ifdef OS_WIN
void hide_filename(std::string&);
#endif
}

#endif //_FILESYSTEM_HPP
