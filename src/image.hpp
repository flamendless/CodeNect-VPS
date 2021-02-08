#ifndef IMAGE_H
#define IMAGE_H

#include <GLFW/glfw3.h>
#include <string>
#include "imgui.h"

namespace CodeNect
{
struct Image
{
	int width = 0;
	int height = 0;
	GLuint texture = 0;
	std::string filename;
	ImVec2 size;
	bool is_hovered = false;

	void* get_texture()
	{
		return (void*)(intptr_t) texture;
	}
};
}

#endif //IMAGE_H
