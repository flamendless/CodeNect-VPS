#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION

#include "filesystem.h"

namespace Filesystem
{
bool open_project_file(std::string& project_filepath)
{
	nfdchar_t* out_path = NULL;
	nfdresult_t result = NFD_OpenDialog(Filesystem::filter_project, NULL, &out_path);

	if (result == NFD_OKAY)
	{
		fmt::printf("Opened project file path: %s\n", out_path);

		project_filepath = out_path;
		free(out_path);

		return true;
	}
	else if (result == NFD_CANCEL)
		fmt::printf("Open project file cancelled\n");
	else
		fmt::printf("Error: %s\n", NFD_GetError());

	return false;
}

bool load_texture_from_file(const char* filename, GLuint* out, int* width, int* height)
{
	int img_width = 0;
	int img_height = 0;
	unsigned char* img_data = stbi_load(filename, &img_width, &img_height, NULL, 4);

	if (img_data == NULL)
		return false;

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

	*out = img_texture;
	*width = img_width;
	*height = img_height;

	return true;
}

bool load_images(const std::vector<std::string>& filenames)
{
	for (int i = 0; i < filenames.size(); i++)
	{
		const std::string& filename = filenames[i];
		fmt::printf("filename: %s\n", filename);
	}

	return true;
}
}
