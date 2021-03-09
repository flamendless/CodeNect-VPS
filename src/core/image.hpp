#ifndef IMAGE_HPP
#define IMAGE_HPP

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

	void* get_texture(void) { return (void*)(intptr_t) texture; }
};
}

#endif //IMAGE_HPP
