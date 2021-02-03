#include "sidebar.hpp"

namespace Sidebar
{
void init()
{
	Filesystem::load_images(CodeNect::Config::sidebar_images_filenames);

	// int img_width = 0;
	// int img_height = 0;
	// unsigned int img_texture = 0;
	// bool ret = Filesystem::load_texture_from_file("assets/test.png", &img_texture, &img_width, &img_height);
    //
	// if (ret)
	// 	fmt::printf("Image loaded successfully\n");
	// else
	// 	fmt::printf("Image load failed\n");
}

void draw()
{

}
}
