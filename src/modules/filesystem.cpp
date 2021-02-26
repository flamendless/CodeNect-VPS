#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION

#include "filesystem.hpp"

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
bool open_project_file(std::string& project_filepath)
{
	nfdchar_t* out_path = NULL;
	nfdresult_t result = NFD_OpenDialog(PROJECT_EXT, NULL, &out_path);

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

int load_texture_from_file(const char* filename, CodeNect::Image& image)
{
	PLOGV << "Loading texture from file: " << filename;

	int img_width = 0;
	int img_height = 0;
	unsigned char* img_data = stbi_load(filename, &img_width, &img_height, NULL, 4);

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
}
}
