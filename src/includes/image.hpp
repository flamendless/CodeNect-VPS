#ifndef IMAGE_H
#define IMAGE_H

#include <GLFW/glfw3.h>
#include <string>
#include "imgui.h"

namespace CodeNect
{
struct Image
{
	std::string filename;
	GLuint texture = 0;

	int width = 0;
	int height = 0;
	ImVec2 size;

	void* get_texture() { return (void*)(intptr_t) texture; }
};
}

#endif //IMAGE_H
