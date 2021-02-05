#ifndef IMAGE_H
#define IMAGE_H

#include <SDL_opengl.h>
#include <string>

namespace CodeNect
{
struct Image
{
	int width = 0;
	int height = 0;
	GLuint texture = 0;
	std::string filename;

	void* get_texture()
	{
		return (void*)(intptr_t) texture;
	}
};
}

#endif //IMAGE_H
