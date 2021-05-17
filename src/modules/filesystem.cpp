#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION

#include "modules/filesystem.hpp"

#include <fstream>
#include <map>
#include <filesystem>
#include <GLFW/glfw3.h>
#include "nfd.h"
#include "fmt/printf.h"
#include "plog/Log.h"
#include "stb_image.h"
#include "core/defines.hpp"

#define GL_CLAMP_TO_EDGE 0x812F

namespace CodeNect::Filesystem
{
std::filesystem::path cur_path = std::filesystem::current_path();
std::string current_path = cur_path.string();

bool open_project_file(std::string& project_filepath)
{
	nfdchar_t* out_path = NULL;
	nfdresult_t result = NFD_OpenDialog(PROJECT_EXT, current_path.c_str(), &out_path);

	if (result == NFD_OKAY)
	{
		PLOGV << "Opened project file path: " << out_path;

		project_filepath = out_path;
		free(out_path);

		return true;
	}
	else if (result == NFD_CANCEL)
		PLOGV << "Open project file cancelled";
	else
		PLOGE << "Error: " << NFD_GetError();

	return false;
}

bool open_filepath(std::string& filepath)
{
	nfdchar_t* out_path = NULL;
	nfdresult_t result = NFD_SaveDialog(PROJECT_EXT, current_path.c_str(), &out_path);

	if (result == NFD_OKAY)
	{
		PLOGV << "Save project file path: " << out_path;

		filepath = out_path;
		free(out_path);

		return true;
	}
	else if (result == NFD_CANCEL)
		PLOGV << "Save project file cancelled";
	else
		PLOGE << "Error: " << NFD_GetError();

	return false;
}

bool save_to_file(std::string& out_filepath, const char* ext, const std::string& content)
{
	nfdchar_t* out_path = NULL;
	nfdresult_t result = NFD_SaveDialog(ext, current_path.c_str(), &out_path);

	if (result == NFD_OKAY)
	{
		PLOGV << "Saved " << ext << " file to:" << out_path;
		out_filepath = out_path;
		free(out_path);

		std::ofstream out(out_filepath + "." + ext);
		out << content;
		out.close();

		return true;
	}
	else if (result == NFD_CANCEL)
		PLOGV << "Save " << ext << " file cancelled";
	else
		PLOGE << "Error: " << NFD_GetError();

	return false;
}

int load_texture_from_file(const char* filename, CodeNect::Image& image)
{
	PLOGV << "Loading texture from file: " << filename;

	int img_width = 0;
	int img_height = 0;
	unsigned char* img_data = stbi_load(filename, &img_width, &img_height, NULL, STBI_rgb_alpha);

	PLOGV << "loaded texture from file: " << "width: " << img_width
		<< " , height: " << img_height;

	if (img_data == NULL)
		return RES_FAIL;

	GLuint img_texture;
	glGenTextures(1, &img_texture);
	glBindTexture(GL_TEXTURE_2D, img_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
	stbi_image_free(img_data);

	image.filename = filename;
	image.texture = img_texture;
	image.width = img_width;
	image.height = img_height;
	image.size = ImVec2(img_width, img_height);

	return RES_SUCCESS;
}

unsigned char* load_texture_from_file(const char* filename, GLFWimage& image)
{
	return stbi_load(filename, &image.width, &image.height, 0, STBI_rgb_alpha);
}
}
